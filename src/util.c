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

char** get_content_dir(char* name) {
    int len = strlen(name);
    DIR* dirp = opendir(".");
    struct dirent* dp;
    while ((dp = readdir(dirp)) != NULL)
        if (dp->d_namlen == len && !strcmp(dp->d_name, name)) {
            printf("%s", dp->d_name);
            (void)closedir(dirp);
            return NULL;
        }
    (void)closedir(dirp);
    return NULL;
}
