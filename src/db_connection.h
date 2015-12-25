#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <my_global.h>
#include <mysql.h>
#include <string>
#include <string.h>

using namespace std;

int check_user_existence(const char * username, const char * password);
void insert_user(const char * password, const char * nickname);
void update_user_nickname(const char * username, const char * nickname);

#endif
