#include "uri.h"

static void convert_to_lowercase(char *uri)
{
}

char* get_full_path(uri_t* uri) {
    return uri->path;
}

char *get_extension(char *path)
{
    char *extension = strrchr(path, '.');

    // If there's no period in the path or it's at the end of the string, there's no extension
    if (extension == NULL || extension == path)
    {
        return ""; // Return an empty string to indicate no extension
    }

    // Return a pointer to the character following the period (the extension)
    return extension;
}

uri_t* create_uri(char *fullpath)
{
    uri_t *uri = (uri_t *)malloc(sizeof(struct uri_struct));

    if (uri == NULL)
    {
        fprintf(stderr, "Error allocating URI data structure, %s\n", strerror(errno));
        return NULL;
    }

    if(fullpath == NULL) {
        fprintf(stderr, "Error fullPath is NULL, %s \n", strerror(errno));
        return NULL;
    }

    char* normalized = normalize_path(fullpath, sizeof(fullpath));

    uri->path = normalized != NULL ? normalized : ".";
    uri->protocol = get_protocol(fullpath);
    uri->extension = get_extension(fullpath);

    return uri;
}

int free_uri(uri_t *uri)
{
    if (uri == NULL)
    {
        return -1;
    }

    free(uri->path);
    free(uri);

    return 0;
}

char *normalize_path(char *path_start, size_t len)
{
    char *path = NULL;

    if (path_start != NULL)
    {
        path_start++;
        char *path_end = strstr(path_start, " ");
        if (path_end != NULL)
        {
            size_t path_length = (path_end - path_start);

            path = (char *)malloc(sizeof(int) * (path_length + 1));

            strncpy(path, path_start + 1, path_length - 1);
            path[path_length] = '\0';
        }
    }

    if (path == NULL)
    {
        fprintf(stderr, "Error decoding path %s\n", strerror(errno));
        return NULL;
    }

    fprintf(stdout, "Path requested: %s\n", path);

    if (path[strlen(path) - 1] == '/')
    {
        strcat(path, "index");
    }

    char *fullPath = strcmp(get_extension(path), ".html") ? strcat(path, ".html") : path;

    return fullPath;
}

enum supported_protocol get_protocol(const char *path)
{
    const char *protocol_delimiter = "://";
    const char *protocol_end = strstr(path, protocol_delimiter);

    // If the protocol delimiter is found
    if (protocol_end != NULL)
    {
        // Calculate the length of the protocol
        size_t protocol_length = protocol_end - path;

        // Allocate memory for the protocol string
        char *protocol = (char *)malloc(protocol_length + 1);

        // Copy the protocol string into the newly allocated memory
        strncpy(protocol, path, protocol_length);
        protocol[protocol_length] = '\0'; // Null-terminate the string

        free(protocol);

        if (strcmp(protocol, "https"))
        {
            return https;
        }

        return http;
    }

    // If the protocol delimiter is not found, return NULL
    return http;
}