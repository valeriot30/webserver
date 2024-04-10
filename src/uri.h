#ifndef URI_H
#define URI_H

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include <errno.h>

typedef enum supported_protocol {
    http,
    https
} uri_protocol;

typedef struct uri_struct {
    uri_protocol protocol;
    char* path;
    char* extension;
} uri_t;

uri_t* create_uri(char* fullpath);
int free_uri(uri_t* uri);
char* normalize_path(char* uri, size_t len);
enum supported_protocol get_protocol(const char *path);
char* get_full_path(uri_t* uri);
char *get_extension(char *path);

#endif