#ifndef NET_H
#define NET_H


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>

int create_server_socket(unsigned int port);
int create_client_socket(unsigned int port);

#endif