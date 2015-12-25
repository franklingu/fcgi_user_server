#ifndef BACKEND_CONNECTION_H
#define BACKEND_CONNECTION_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <time.h>

using namespace std;

string communicate_with_backend(string str);

class backend_connection
{
public:
    backend_connection();
    ~backend_connection();
    string communicate_with_backend(string str);
private:
    int sock;
};

#endif
