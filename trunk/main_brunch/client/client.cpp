#include<iostream>
#include<stdio.h>
#include<cstring>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
using namespace std;
#define BUFF_SIZE 1024
#define NAME_SIZE 10
#define PASSWORD_SIZE 10

int socket_id;

void close_client(int)
{
    close(socket_id);
    exit(0);
}

int main(int argc,char**argv)
{
    char send_buffer[BUFF_SIZE]={0};
    char recv_buffer[BUFF_SIZE]={0};
    char client_name[NAME_SIZE]={0};
    char client_password[PASSWORD_SIZE]={0};
    char repeat_password[PASSWORD_SIZE]={0};

    //char service_name[NAME_SIZE={0};
    char service_password[PASSWORD_SIZE]={0};

    char judge[5]={0};//char *judge;

    struct sockaddr_in client_addr;
    memset(&client_addr,0,sizeof(client_addr));
    client_addr.sin_family=AF_INET;
    if(2!=argc)
    {
        cout << "please check in your ip address!" << endl;
        return -1;
    }
    client_addr.sin_port=htons(0x6666);
    if(!(inet_pton(AF_INET,argv[1],&client_addr.sin_addr)>0))//
    {
        cout << "inet_pton failed" << endl;
        return -1;
    }
    socket_id=socket(AF_INET,SOCK_STREAM,0);
    if(-1==socket_id)
    {
        cout << "socket failed" << endl;
        return -1;
    }
    if(-1==connect(socket_id,(struct sockaddr *)&client_addr,sizeof(client_addr)))
    {
        cout << "connect failed" << endl;
        return -1;
    }

    cout << "are you first login? y/n:";
    cin >> judge;
    getchar();
    if(0==strcmp(judge,"y")||0==strcmp(judge,"yes"))
    {
        strcpy(send_buffer,"register");
        if(send(socket_id,send_buffer,sizeof(send_buffer),0)<0)
        {
            cout << "send 'register' fail!" << endl;
            return NULL;
        }
        while(1)
        {
            int last=0;
            char *find_enter=NULL;
            cout << "first login,please set your name:";
            fgets(client_name,NAME_SIZE,stdin);
            find_enter=strchr(client_name,'\n');
            if(find_enter)
                *find_enter='\0';
            last=strlen(client_name);
            if(last>(NAME_SIZE-2))
            {
                cout << "your name is too long,please set again!" << endl;
                continue;
            }
            if(0==last||' '==client_name[last]||'\t'==client_name[last])
            {
                cout << "your name is invaild,please set again!" << endl;
                continue;
            }
            client_name[last]=0;
            memcpy(send_buffer,client_name,NAME_SIZE);
            if(send(socket_id,send_buffer,sizeof(send_buffer),0)<0)
            {
                cout << "send name failed" << endl;
                return -1;
            }
            if(recv(socket_id,recv_buffer,sizeof(recv_buffer),0)<0)
            {
                cout << "recv password information from service failed" << endl;
                return -1;
            }
            if(0!=strcmp(recv_buffer,"-1"))
            {
                cout << "this name has existed" << endl;
                continue;
            }
            break;
        }
        while(1)
        {
            char *find_enter=NULL;
            cout << "first login,please set your password:";
            fgets(client_password,PASSWORD_SIZE,stdin);
            find_enter=strchr(client_password,'\n');
            if(find_enter)
                *find_enter='\0';
            if(strlen(client_password)>PASSWORD_SIZE-1)
            {
                cout << "your password is too long,please set again!" << endl;
                continue;
            }

            cout << "repeat your password:";
            fgets(repeat_password,PASSWORD_SIZE,stdin);
            find_enter=strchr(repeat_password,'\n');
            if(find_enter)
                *find_enter='\0';
            if(0==strcmp(repeat_password,client_password))
            {
                cout << "all sets success" << endl;

                memcpy(send_buffer,client_password,PASSWORD_SIZE);
                if(send(socket_id,send_buffer,sizeof(send_buffer),0)<0)
                {
                    cout << "send password failed" << endl;
                    return -1;
                }
                break;//???
            }
        }
    }
    else if(0==strcmp(judge,"n")||0==strcmp(judge,"no"))
    {
        strcpy(send_buffer,"login");
        if(send(socket_id,send_buffer,sizeof(send_buffer),0)<0)
        {
            cout << "send 'login' fail!" << endl;
            return NULL;
        }
        while(1)
        {
            int last=0;
            char *find_enter=NULL;
            cout << "entry your name:";
            fgets(client_name,NAME_SIZE,stdin);
            find_enter=strchr(client_name,'\n');
            if(find_enter)
                *find_enter='\0';
            last=strlen(client_name);
            client_name[last]=0;
            memcpy(send_buffer,client_name,NAME_SIZE);
            if(send(socket_id,send_buffer,sizeof(send_buffer),0)<0)
            {
                cout << "send name failed" << endl;
                return -1;
            }
            //???
            if(recv(socket_id,recv_buffer,sizeof(recv_buffer),0)<0)
            {
                cout << "recv failed" << endl;
                return -1;
            }
            if(0==strcmp(recv_buffer,"-1"))
            {
                cout << "this name is not exist or this name is online" << endl;
                continue;
            }
            while(1)
            {
                char *find_enter=NULL;
                cout << "entry your password:";
                fgets(service_password,BUFF_SIZE,stdin);
                find_enter=strchr(service_password,'\n');
                if(find_enter)
                    *find_enter='\0';
                cout << service_password << recv_buffer << endl;
                if(0!=strcmp(service_password,recv_buffer))
                {
                    cout << "password is error" << endl;
                    continue;
                }
                else
                {
                    memcpy(send_buffer,client_name,sizeof(client_name));
                    if(send(socket_id,send_buffer,sizeof(send_buffer),0)<0)
                    {
                        cout << "send connected name failed" << endl;
                    }
                    while(1)
                    {
                        cout << client_name << " : ";
                        signal(SIGINT,close_client);
                        fgets(send_buffer,BUFF_SIZE,stdin);
                        if(send(socket_id,send_buffer,sizeof(send_buffer),0)<0)
                        {
                            cout << "send failed" << endl;
                            return -1;
                        }
                    }
                }
            }
        }
    }
    else
    {
        cout << "please choose 'y/yes'or'n/no" << endl;
    }
}
