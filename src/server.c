
#include "server.h"

sem_t resources;

#ifdef __APPLE__
    sem_t resources = 1;
#endif


void write_content_to_socket(int sockfd, const char *content, const char* mime_type)
{
    char length_str[BUFF_MAX_SND];
    sprintf(length_str, "%d", (int)strlen(content));

    char *content_length_str = strcat(length_str, "Content-Length: ");
    write_ln_to_socket(sockfd, "Server: WebServer/1.0 (MacOS)");
    write_ln_to_socket(sockfd, CONTENT_TYPE(mime_type));
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

    char* extension = get_extension(fullPath);

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
            fprintf(stderr, "[error] Cannot read file %s\n", strerror(errno));
        #endif
    }

    write_ln_to_socket(*client_socket_id, request_to_send); // Request line

    printf("mime: %s", extension);

    write_content_to_socket(*client_socket_id, content, get_mime_from_type(extension));

    goto close_conn;

    close_conn:
        // we flush buffer
        free_uri(uri);
        close(*client_socket_id);

    return NULL;
}

int create_http_server(t_config configfd)
{

    #ifndef __APPLE__
        sem_init(&resources, 1, 1);
    #endif

    unsigned int port = configfd.port;

    fprintf(stdout, "[server] Starting webserver on port %d\n", port);

    thread_pool_t pool;

    if(!initialize_threadpool(&pool, 8)) 
    {
        fprintf(stderr, "[error] Failed to initialize thread pool %s\n", strerror(errno));
    }

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
    
    destroy_threadpool(&pool);
    
    close(sd);

    return sd;
}