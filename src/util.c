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
int get_file_size(char* name) {

    FILE* fp = fopen(name, "rb");

    if(fp == NULL) return 1;

    fseek(fp, 0L, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    fclose(fp);

    return sz;
}

pid_t execute(const char *command, FILE **in, FILE **out, FILE **err)
{
    pid_t pid;
    int   fd[6];

    pipe(&fd[0]);
    pipe(&fd[2]);
    pipe(&fd[4]);

    switch (pid = fork()) {
        case -1:
            perror("unable to fork()");
            exit(1);

        case 0:
            close(fd[1]);   // Close write end of stdin.
            close(fd[2]);   // Close read end of stdout.
            close(fd[4]);   // Close read end of stderr.

            dup2(fd[0], STDIN_FILENO);  // Have stdin read from the first pipe.
            dup2(fd[3], STDOUT_FILENO); // Have stdout write to the second pipe.
            dup2(fd[5], STDERR_FILENO); // Have stderr write to the third pipe.

            execlp("/bin/sh", "/bin/sh", "-c", command, (char *) NULL);

            perror("execlp() failed");
            _exit(1);

        default:
            close(fd[0]); // Close read end of stdin.
            close(fd[3]); // Close write end of stdout.
            close(fd[5]); // Close write end of stderr.

            if (in)  *in  = fdopen(fd[1], "wb"); else close(fd[1]);
            if (out) *out = fdopen(fd[2], "rb"); else close(fd[2]);
            if (err) *err = fdopen(fd[4], "rb"); else close(fd[4]);

            return pid;
    }
}

char* execute_php_script(const char *php_script, const char *php_file) {
    char command[256];
    snprintf(command, sizeof(command), "php %s %s", php_script, php_file);

    FILE *fp;
    char result[1024];

    // Open the command for reading
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error opening pipe");
        return NULL;
    }

    // Read the output a line at a time
    while (fgets(result, 1024, fp) != NULL) {
        //printf("%s", result);
    }

    // Close the pipe
    pclose(fp);

    return (char*) result;
}