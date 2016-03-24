#ifndef DATABASEOPERA_H
#define DATABASEOPERA_H

#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<string.h>

#include"mysql_connection.h"
#include<cppconn/config.h>
#include<cppconn/exception.h>
#include<cppconn/resultset.h>
#include<cppconn/statement.h>
#include<cppconn/driver.h>
using namespace sql;
using namespace std;
class DatabaseOpera
{
    public:
        DatabaseOpera();
        virtual ~DatabaseOpera();
        string check_db(string database,string instruct);
        int insr_dele_db(string database,string instruct);

    private:
};

#endif // DATABASEOPERA_H
