#ifndef UTIL_H
#define UTIL_H

#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include <dirent.h>
#include <math.h>

#include "base64.h"

static inline int int2buffer(int x) {
    return (int)((ceil(log10(x))+1)*sizeof(char));
}

char* str_safe_concat(const char *s1, const char *s2);
int get_file_size(char* name);
char *rtrim(char *str);
char** get_content_dir(char* name);
char* execute_php_script(const char *php_script, const char *php_file);

#endif