#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>

#include "./headers/write_handler.h"
#include "./headers/read_handler.h"

#define MAX_COMMAND_SIZE 256

using namespace std;

extern int errno;

int port;

/* socket descriptor */
int sd;

bool alive = true;

bool authenticated = false;

char myUsername[256];


int main (int argc, char *argv[])
{
  struct sockaddr_in server;
  pthread_t th_read;
  pthread_t th_write;

  port = atoi ("2908");

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  server.sin_family = AF_INET; /* internetwork: UDP, TCP, etc. */

  server.sin_addr.s_addr = inet_addr("127.0.0.1");

  server.sin_port = htons (port);

  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

  pthread_create(&th_read, NULL, &read_handler::readFromServer, NULL);
  pthread_create(&th_write, NULL, &write_handler::writeToServer, NULL);
  while(alive){}
}