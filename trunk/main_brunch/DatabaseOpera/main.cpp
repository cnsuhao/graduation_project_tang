//服务端程序
#include<iostream>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include"DatabaseOpera.h"

#define PORT_NUMBER 0x6666
#define BUFF_SIZE 1024
#define NAME_SIZE 10
#define PASSWORD_SIZE 10
using namespace std;
string database="graduation_project_database";
string all_table="all_clients_list";
string online_table="online_clients_list";

char * check_name(char * name,string table)
{
    DatabaseOpera opera_db;
    string output;
    string instruct;
    string name_str=name;
    char * result=new char[10];
    char * result_str=NULL;
    if(table==all_table)
    {
        instruct="select client_password from "+table+" where client_name='"+name_str+"'";
    }
    else if(table==online_table)
    {
        instruct="select online_client_name from "+table+" where online_client_name='"+name_str+"'";
    }
    cout << instruct << endl;
    output=opera_db.check_db(database,instruct);
    result_str=(char *)output.c_str();
    memcpy(result,result_str,10);
    //if(result==NULL)
        //cout << "get NULL" << endl;
    return result;
}

void insert_user_list(char *name,char *password)
{
    DatabaseOpera opera_db;
    string name_str=name;
    string password_str=password;
    string instruct="insert into all_clients_list(client_name,client_password)values('"+name_str+"','"+password_str+"')";
    if(0==opera_db.insr_dele_db(database,instruct))
    {
        cout << "insert user_list fail!" << endl;
        exit(0);
    }
    else
    {
        cout << "insert user_list success!" << endl;
    }
}

void insert_user_online(char * socket_id,char * name)
{
    DatabaseOpera opera_db;
    string socket_id_str=socket_id;
    string name_str=name;
    string instruct="insert into online_clients_list(connect_socket_id,online_client_name)values('"+socket_id_str+"','"+name_str+"')";
    if(0==opera_db.insr_dele_db(database,instruct))
    {
        cout << "insert user_online fail!" << endl;
        exit(0);
    }
    else{
        cout << "insert user_online success!" << endl;
    }
}

void * receive(void * arg)
{
    int num=0;
    char recv_buffer[BUFF_SIZE]={0};
    char send_buffer[BUFF_SIZE]={0};

    char name[NAME_SIZE]={0};
    char password[PASSWORD_SIZE]={0};

    char * name_pointer=NULL;
    char * password_pointer=NULL;
    char * online_name=NULL;

    char connect_id_str[100]={0};

    int connect_id=*((int *)arg);
    if(recv(connect_id,recv_buffer,BUFF_SIZE,0)<0)
    {
        cout << "get register/login from client fail!" << endl;
        return NULL;
    }

    if(0==strcmp(recv_buffer,"register"))
    {
        while(1)
        {
            if(recv(connect_id,recv_buffer,BUFF_SIZE,0)<0)
            {
                cout << "when register,recv name fail!" << endl;
                return NULL;
            }
            name_pointer=check_name(recv_buffer,all_table);
            memcpy(name,recv_buffer,NAME_SIZE);
            memcpy(send_buffer,name_pointer,NAME_SIZE);
            if(send(connect_id,send_buffer,BUFF_SIZE,0)<0)
            {
                cout << "when register,send password fail!" << endl;
                return NULL;
            }
            if(0==strcmp(name_pointer,"-1"))
            {
                break;
            }
        }
        if(recv(connect_id,recv_buffer,BUFF_SIZE,0)<0)
        {
            cout << "when register,recv password fail!" << endl;
            return NULL;
        }
        memcpy(password,recv_buffer,strlen(recv_buffer));
        insert_user_list(name,password);
    }
    else
    {
        if(recv(connect_id,recv_buffer,BUFF_SIZE,0)<0)
        {
            cout << "recv name failed!" << endl;
            return NULL;
        }

        password_pointer=check_name(recv_buffer,all_table);
        online_name=check_name(recv_buffer,online_table);
        if((0!=strcmp(password_pointer,"-1"))&&(0==strcmp(online_name,"-1")))
        {
            memcpy(send_buffer,password_pointer,PASSWORD_SIZE);
        }
        else
        {
            memcpy(send_buffer,"-1",PASSWORD_SIZE);
        }
        if(send(connect_id,send_buffer,BUFF_SIZE,0)<0)
        {
            cout << "service send failed" << endl;
            return NULL;
        }
        delete [] password_pointer;

        if(recv(connect_id,recv_buffer,sizeof(recv_buffer),0)<0)
        {
            cout << "recv connected name failed!" << endl;
            return NULL;
        }
        sprintf(connect_id_str,"%d",connect_id);
        insert_user_online(connect_id_str,recv_buffer);

        cout << recv_buffer <<" on" << endl;

        while(1)
        {
            if((num=recv(connect_id,recv_buffer,BUFF_SIZE,0))<0)
            {
                cout << "recv message failed!" << endl;
                return NULL;
            }
            if (num!=0)
            {
                cout << recv_buffer << endl;
            }
            else
            {
                return NULL;
            }
        }
    }

    return NULL;
}

int main()
{
    int socket_id;
    int socket_connect_id;
    struct sockaddr_in s_add;
    memset(&s_add,0,sizeof(s_add));
    s_add.sin_family=AF_INET;
    s_add.sin_port=htons(PORT_NUMBER);
    s_add.sin_addr.s_addr=htons(INADDR_ANY);
    socket_id=socket(AF_INET,SOCK_STREAM,0);

    pthread_t pid=0;
    pthread_attr_t attr={0};
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr,PTHREAD_SCOPE_PROCESS);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    int client_connect_number=0;

    if(socket_id==-1)
    {
        cout << "创建socket失败" << endl;
        return -1;
    }
    if(-1==bind(socket_id,(struct sockaddr *)&s_add,sizeof(s_add)))
    {
        cout << "bind failed" << endl;
        return -1;
    }
    if(-1==listen(socket_id,5))
    {
        cout << "listen failed" << endl;
        return -1;
    }
    cout << "--------listen client's request---------" << endl;
    while(1)
    {
        socket_connect_id=accept(socket_id,(struct sockaddr *)NULL,NULL);
        if(-1==socket_connect_id)
        {
            cout << "accept failed" << endl;
            return -1;
        }
        if(0!=pthread_create(&pid,&attr,receive,(void *)&socket_connect_id))
        {
            cout << "pthread_create failed!" << endl;
            return -1;
        }
        client_connect_number++;
    }
    close(socket_id);
}




/*int main()
{
    int number=0;
    string command;
    DatabaseOpera opera_db;
    char * name="tang";
    char * password=NULL;
    password=check_name(name,all_table);
    cout << password << endl;
    *out << "please input SQL command : ";
    getline(cin,command,'\n');
    number=opera_db.insr_dele_db(database,command);
    if(number==0)
    {
        cout << "work failed" << endl;
    }
    else
    {
        cout << "work success!";
    }*
    return 0;
}*/
