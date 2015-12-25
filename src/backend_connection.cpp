#include "backend_connection.h"

backend_connection::backend_connection()
{
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    int destPort = 5432;

    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(5432);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*---- Connect the socket to the server using the address struct ----*/
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
    this->sock = clientSocket;
}

backend_connection::~backend_connection()
{
    close(sock);
}

string backend_connection::communicate_with_backend(string str)
{
    char buffer[1024];
    strcpy(buffer, str.c_str());
    send(sock, buffer, strlen(buffer), 0);
    /*---- Read the message from the server into the buffer ----*/
    recv(sock, buffer, 1024, 0);

    return string(buffer);
}
