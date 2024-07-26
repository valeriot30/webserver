#ifndef SERVER_RESPONSE_H
#define SERVER_RESPONSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "util.h"

#define HTTP_RESPONSE_OK "HTTP/1.1 200 OK"
#define HTTP_RESPONSE_404 "HTTP/1.1 404 NOT FOUND"

#define RESPONSE_CONTENT_TYPE "Content-Type: "
#define RESPONSE_CONTENT_LENGTH "Content-Length: "
#define RESPONSE_ACCEPT_RANGE "Accept-Ranges: bytes"

enum response_result {
    ERROR_ALLOC_RESPONSE,
    ALLOC_SUCCESS
};

typedef struct response {
    char* content;
    bool is_text;
    char* mime_type;
    int content_length;
    char* response_line;
} response_t;

int alloc_response(response_t** response, char* content, char* mime_type, bool is_text, int content_length, char* response_line);
int free_response(response_t* response);
char* get_response_mime_str(response_t* response);
char* get_content_length_as_string(response_t* response);
int get_content_length(response_t* response);
char* get_content_str(response_t* response);
char* get_content_length_str(response_t* response);
char* get_response_mime_type(response_t* response);
bool is_response_text(response_t* response);
char* get_response_line(response_t* response);
#endif