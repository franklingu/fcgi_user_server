#include "db_connection.h"

using namespace std;

db_connection::db_connection()
{
    this->connection = mysql_init(NULL);
    connect_to_mysql();
}

db_connection::~db_connection()
{
    mysql_close(connection);
}

void db_connection::execute_query(const char *query)
{
    MYSQL *con = this->connection;
    printf("Query: %s\n", query);
    if (mysql_query(con, query))
    {
        string err(mysql_error(con));
        throw std::runtime_error("Connection Error: " + err);
    }
}

MYSQL *db_connection::connect_to_mysql()
{
    MYSQL *con = this->connection;
    if (con == NULL)
    {
        printf("DB error: %s\n", mysql_error(con));
        exit(1);
    }
    if (mysql_real_connect(con, "localhost", "root", "", NULL, 0, NULL, 0) == NULL)
    {
        string err(mysql_error(con));
        throw std::runtime_error("Connection Error: " + err);
    }
    execute_query("use entry_task;");
    return con;
}

int db_connection::check_user_existence(const char * username, const char * password)
{
    string username_s(username, strlen(username));
    string password_s(password, strlen(password));
    string query = "select username from users where username = " + username_s +
        " and password = '" + password_s + "';";
    execute_query(query.c_str());
    MYSQL_RES *result = mysql_store_result(connection);
    if (result != NULL)
    {
        int num_rows = mysql_num_rows(result);
        mysql_free_result(result);
        return (num_rows == 0) ? -1 : atoi(username);
    }
    else
    {
        return -1;
    }
}

void db_connection::insert_user(const char * password, const char * nickname)
{
    string nickname_s(nickname, strlen(nickname));
    string password_s(password, strlen(password));
    string query = "insert into users(password, nickname) values('" + password_s +
        "', '" + nickname_s + "');";
    execute_query(query.c_str());
}

void db_connection::update_user_nickname(const char * username, const char * nickname)
{
    string nickname_s(nickname, strlen(nickname));
    string username_s(username, strlen(username));
    string query = "update users set nickname = '" + nickname_s +
        "' where username = " + username_s + ";";
    execute_query(query.c_str());
}
