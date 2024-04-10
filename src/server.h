#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

#include "config.h"
#include "net.h"
#include "thread.h"
#include "config.h"
#include "buffer.h"
#include "response.h"
#include "uri.h"
#include "mime.h"

#define BUFF_MAX_RCV 65536 // 64K
#define BUFF_MAX_SND 262144 // 256K 

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#define RESPONSE_CONTENT_TYPE "Content-Type: "
#define RESPONSE_CONTENT_LENGTH "Content-Length: "

int create_http_server(t_config configfd);
void write_ln_to_socket(int sockfd, const char *message);
void write_content_to_socket(int sockfd, const char *content, const char* mime_type);

#endif