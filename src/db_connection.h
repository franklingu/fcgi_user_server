#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <my_global.h>
#include <mysql.h>
#include <string>
#include <string.h>
#include <stdexcept>

using namespace std;

class db_connection
{
public:
    db_connection();
    ~db_connection();
    int check_user_existence(const char * username, const char * pswd);
    void insert_user(const char * password, const char * nickname);
    void update_user_nickname(const char * username, const char * nickname);
private:
    MYSQL *connection;
    MYSQL *connect_to_mysql();
    void execute_query(const char *query);
};

#endif
