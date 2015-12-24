#ifndef UPDATE_NICK_FORM_H
#define UPDATE_NICK_FORM_H

#include <my_global.h>
#include <mysql.h>
#include <iostream>
#include <string>

using namespace std;
bool check_user_existence(string username, string password);
void insert_user(string password, string nickname);
void update_user_nickname(string username, string nickname);

#endif
