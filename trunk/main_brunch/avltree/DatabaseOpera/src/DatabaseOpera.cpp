#include "DatabaseOpera.h"
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string.h>

#include"mysql_connection.h"
#include<cppconn/driver.h>
#include<cppconn/exception.h>
#include<cppconn/resultset.h>
#include<cppconn/statement.h>
using namespace std;
using namespace sql;
using namespace mysql;

#define DBHOST "tcp://192.168.137.1:3306"
#define USER "root"
#define PASSWORD "930905"

DatabaseOpera::DatabaseOpera()
{
    //ctor
}

DatabaseOpera::~DatabaseOpera()
{
    //dtor
}

string DatabaseOpera::check_db(string database,string instruct)
{
    string output;
    try{
            Driver * driver=NULL;
            Connection * con=NULL;
            Statement * state=NULL;
            ResultSet * result=NULL;

            driver=get_driver_instance();
            con=driver->connect(DBHOST,USER,PASSWORD);
            con->setSchema(database);
            state=con->createStatement();
            //str="selcet from"+table+"where client_name="+name;
            result=state->executeQuery(instruct);
            if(result->next())
            {
                output=result->getString(2);
            }
            else{
                output="-1";
            }
            delete con;
            delete state;
            delete result;
        }
        catch(SQLException &e)
        {
            /*cout << "# ERR: SQLException in " << _FILE_;
            cout << "<"_FUNCTION_">" << _LINE_ << endl;*/
            cout << "# ERR:" << e.what();
            cout << "MYSQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << endl;
        }

        return output;
}

int DatabaseOpera::insr_dele_db(string database,string instruct)
{
    int output=0;
    try{
        Driver * driver=NULL;
        Connection * con=NULL;
        Statement * state=NULL;
        ResultSet * result=NULL;

        driver=get_driver_instance();
        con=driver->connect(DBHOST,USER,PASSWORD);
        con->setSchema(database);
        state=con->createStatement();
        //str="insert into "+table+"(client_name,client_password) values("+"'"+name+"'"+","+"'"+password+"'"+")";
        output=state->executeUpdate(instruct);

        delete result;
        delete state;
        delete con;
       }
    catch(SQLException &e)
    {
        /*cout << "# ERR: SQLException in " << _FILE_;
        cout << "<'_FUNCTION_'>" << _LINE_ << endl;*/
        cout << "# ERR:" << e.what();
        cout << "MYSQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << endl;
    }
    return output;
}







