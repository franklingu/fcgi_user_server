#include "db_connection.h"

using namespace std;

void finish_with_error(MYSQL *con)
{
    printf("Connection error: %s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

void execute_query(MYSQL *con, const char *query)
{
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }
}

MYSQL *connect_to_mysql()
{
    MYSQL *con = mysql_init(NULL);
    if (con == NULL)
    {
        printf("DB error: %s\n", mysql_error(con));
        exit(1);
    }
    if (mysql_real_connect(con, "localhost", "root", "", NULL, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }
    execute_query(con, "use entry_task;");
    return con;
}

int check_user_existence(const char * username, const char * password)
{
    MYSQL *con = connect_to_mysql();
    string username_s(username, strlen(username));
    string password_s(password, strlen(password));
    string query = "select username from users where username = " + username_s +
        " and password = '" + password_s + "';";
    execute_query(con, query.c_str());
    MYSQL_RES *result = mysql_store_result(con);
    if (result != NULL) {
        int num_rows = mysql_num_rows(result);
        mysql_free_result(result);
        mysql_close(con);
        return (num_rows == 0) ? -1 : 0;
    } else {
        mysql_close(con);
        return -1;
    }
}

void insert_user(const char * password, const char * nickname)
{
    MYSQL *con = connect_to_mysql();

    string nickname_s(nickname, strlen(nickname));
    string password_s(password, strlen(password));
    string query = "insert into users(password, nickname) values('" + password_s +
        "', '" + nickname_s + "');";

    mysql_close(con);
}

void update_user_nickname(const char * username, const char * nickname)
{
    MYSQL *con = connect_to_mysql();

    string nickname_s(nickname, strlen(nickname));
    string username_s(username, strlen(username));
    string query = "update users set nickname = '" + nickname_s +
        "' where username = " + username_s + ";";
    execute_query(con, query.c_str());

    mysql_close(con);
}
