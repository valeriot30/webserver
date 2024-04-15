#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "stdbool.h"

struct buffer {
    char    *content;
    int     len;
};

typedef struct buffer buffer_t;

bool get_content_from_file(char* filename, char** buffer);
int allocate_buffer(int len, buffer_t* buf);
int destroy_buffer(buffer_t buffer);
int flush_content_from_buffer(char* buffer);

#endif