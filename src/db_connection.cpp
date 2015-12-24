#include "db_connection.h"

using namespace std;

void finish_with_error(MYSQL *con)
{
    cout << "Connection Error: " << mysql_error(con) << endl;
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
        cout << mysql_error(con) << endl;
        exit(1);
    }
    if (mysql_real_connect(con, "localhost", "root", "", NULL, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }
    execute_query(con, "use entry_task;");
    return con;
}

bool check_user_existence(string username, string password)
{
    MYSQL *con = connect_to_mysql();

    string query = "select username from users where username = " + username +
        " and password = '" + password + "';";
    execute_query(con, query.c_str());
    MYSQL_RES *result = mysql_store_result(con);
    if (result != NULL) {
        int num_rows = mysql_num_rows(result);
        mysql_free_result(result);
        mysql_close(con);
        return num_rows != 0;
    } else {
        mysql_close(con);
        return false;
    }
}

void insert_user(string password, string nickname)
{
    MYSQL *con = connect_to_mysql();

    string query = "insert into users(password, nickname) values('" + password +
        "', '" + nickname + "');";
    execute_query(con, query.c_str());

    mysql_close(con);
}

void update_user_nickname(string username, string nickname)
{
    MYSQL *con = connect_to_mysql();

    string query = "update users set nickname = '" + nickname +
        "' where username = " + username + ";";
    execute_query(con, query.c_str());

    mysql_close(con);
}

// int main()
// {
//     if (check_user_existence("1", "pswd")) {
//         cout << "1 with pswd exists" << endl;
//     }
//     if (check_user_existence("1", "pswd1")) {
//         cout << "1 with pswd1 exists" << endl;
//     } else {
//         cout << "1 with pswd1 does not exist" << endl;
//     }
//     insert_user("pswd", "test insert");
//     update_user_nickname("2", "test update");

//     return 0;
// }
