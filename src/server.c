
#include "server.h"

sem_t resources;

void write_content_to_socket(int sockfd, const char *content)
{
    char length_str[BUFF_MAX_SND];
    sprintf(length_str, "%d", (int)strlen(content));

    char *content_length_str = strcat(length_str, "Content-Length: ");
    write_ln_to_socket(sockfd, "Server: WebServer/1.0 (MacOS)");
    write_ln_to_socket(sockfd, "Content-Type: text/html");
    write_ln_to_socket(sockfd, content_length_str);
    write_ln_to_socket(sockfd, "");
    write_ln_to_socket(sockfd, content);
}

void write_ln_to_socket(int sockfd, const char *message)
{
    write(sockfd, message, strlen(message));
    write(sockfd, "\r\n", 2); // CRLF
}

inline static void* new_client_instance(void* new_socket)
{
    sig_atomic_t* client_socket_id = (int*) new_socket;

    char request[BUFF_MAX_RCV];

    // Read request
    int bytes_recvd = recv(*client_socket_id, request, BUFF_MAX_RCV - 1, 0);

    if (bytes_recvd < 0)
    {
        fprintf(stderr, "Error receiving request %s\n", strerror(errno));
        goto close_conn;
    }

    char *request_to_send = HTTP_RESPONSE_OK;
    char *path_start = strstr(request, " ");

    uri_t* uri = create_uri(path_start);  

    if(uri == NULL) {
        fprintf(stderr, "Error allocating uri %s\n", strerror(errno));
        goto close_conn;
    }

    char *fullPath = get_full_path(uri);

    if(fullPath == NULL)
        goto close_conn;

    fprintf(stdout, "Serving static file: %s\n", fullPath);

    char *content = 0;

    int result = get_content_from_file(fullPath, &content);

    if (!result)
    {
        fprintf(stdout, "Couldn't find entry %s\n", fullPath);

        if (strcmp(fullPath, "index.html") == 0)
        {
            // explore
            // get_content_dir(".");
            content = "<html>List of all directories</html>";
        }
        else
        {
            content = "<html>404 error</html>";
            request_to_send = HTTP_RESPONSE_404;
        }
    }
    else {
        #ifdef FILE_DEBUG 
            fprintf(stderr, "Cannot read file %s\n", strerror(errno));
        #endif
    }

    write_ln_to_socket(*client_socket_id, request_to_send);
    write_content_to_socket(*client_socket_id, content);

    goto close_conn;

    close_conn:
        // we flush buffer
        free(content);
        free_uri(uri);
        close(*client_socket_id);

    return NULL;
}

int create_http_server(t_config configfd)
{

    sem_init(&resources, 1, 1);

    unsigned int port = configfd.port;

    fprintf(stdout, "Starting webserver on port %d\n", port);

    int sd = create_server_socket(port);

    while (1)
    {
        fflush(stdout);

        sem_wait(&resources);

        int new_socket = create_client_socket(port);

        pthread_t id;

        pthread_create(&id, NULL, new_client_instance, (void*) &new_socket);
        pthread_join(id, NULL);

        sem_post(&resources);
    }

    close(sd);

    return sd;
}