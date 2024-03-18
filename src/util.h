#ifndef UTIL_H
#define UTIL_H

#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "dirent.h"

char *rtrim(char *str);
char** get_content_dir(char* name);

#endif