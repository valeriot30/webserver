#include "util.h"

/**
 * @brief Trim a string by removing trailing whitespaces.
 * 
 * @param str 
 * @return char* 
 */
char *rtrim(char *str)
{
    char *pstart;
    char *p;

    pstart = str;
    while (isspace(*pstart))
        pstart++;

    if (pstart > str)
    {
        p = str;
        while(*pstart)
        {
            *p = *pstart;
            pstart++;
            p++;
        }
        *p = '\0';
    }

    return str;
}

char* str_safe_concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char** get_content_dir(char* name) {
    return NULL;
}
