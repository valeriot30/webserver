#ifndef UTIL_H
#define UTIL_H

#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "dirent.h"

char* str_safe_concat(const char *s1, const char *s2);
long get_file_size(FILE* f);
char *rtrim(char *str);
char** get_content_dir(char* name);

#endif