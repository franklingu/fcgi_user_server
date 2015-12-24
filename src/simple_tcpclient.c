#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  int clientSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  int destPort = 5432;

  if (argc != 2)
  {
    fprintf (stderr, "Usage: %s [port]\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  destPort = atoi(argv[1]);

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

  strcpy(buffer, "Hello World\n");
  send(clientSocket, buffer, 13, 0);
  /*---- Read the message from the server into the buffer ----*/
  recv(clientSocket, buffer, 1024, 0);

  /*---- Print the received message ----*/
  printf("Data received: %s",buffer);

  return 0;
}
