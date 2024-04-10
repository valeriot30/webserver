#ifndef SERVER_RESPONSE_H
#define SERVER_RESPONSE_H

#include <string.h>
#include <stdbool.h>

#define HTTP_RESPONSE_OK "HTTP/1.1 200 OK"
#define HTTP_RESPONSE_404 "HTTP/1.1 404 NOT FOUND"
#define RESPONSE_CONTENT_TYPE "Content-Type: "
#define RESPONSE_CONTENT_LENGTH "Content-Length: "

typedef struct response {
    char* content;
    bool is_text;
    char* mime_type;
    int content_length;
    char* response_line;
} response_t;

#endif