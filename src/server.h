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

#include "logger.h"
#include "config.h"
#include "net.h"
#include "thread.h"
#include "config.h"
#include "buffer.h"
#include "response.h"
#include "uri.h"
#include "mime.h"

#define VERSION "1.0"

#define BUFF_MAX_RCV 65536 // 64K
#define BUFF_MAX_SND 262144 // 256K 

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#define concat0(str1, str2) (str1 "" str2 " ")

#define SERVER_PRODUCT_NAME concat0("Server: WebServer/", VERSION)

// since we support only macos and linux (for posix standard)
static inline char* get_hostname_os() {
    #ifdef __APPLE__
        return "MacOS";
    #else 
        return "Linux";
    #endif

    return "(null)";
}

int create_http_server(t_config configfd);
void write_ln_to_socket(int sockfd, char *message, int len);
void write_response_to_socket(int sockfd, response_t* response);

#endif