
#include "server.h"

sem_t resources;

#ifdef __APPLE__
    sem_t resources = 1;
#endif

void write_content_to_socket(int sockfd, const char *content, const char* mime_type, const long fileSize)
{
    char length_str[BUFF_MAX_SND];
    memset(length_str, 0, sizeof length_str);
    sprintf(length_str, "%d", (int)strlen(content));

    char *content_length_str = str_safe_concat(RESPONSE_CONTENT_LENGTH, length_str);
    char *content_type = str_safe_concat(RESPONSE_CONTENT_TYPE, mime_type);
    char* content_server = str_safe_concat(SERVER_PRODUCT_NAME, get_hostname_os());
    write_ln_to_socket(sockfd, content_server);
    write_ln_to_socket(sockfd, content_type);
    write_ln_to_socket(sockfd, content_length_str);
    write_ln_to_socket(sockfd, "");
    write_ln_to_socket(sockfd, content);

    free(content_length_str);
    free(content_server);
    free(content_type);
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
        ERROR_LOG("[error] Error receiving request %s", strerror(errno));
        goto close_conn;
    }

    char *request_to_send = HTTP_RESPONSE_OK;
    char *path_start = strstr(request, " ");

    uri_t* uri = create_uri(path_start);  

    if(uri == NULL) {
        ERROR_LOG("[error] Error allocating uri %s", strerror(errno));
        goto close_conn;
    }

    char *fullPath = get_full_path(uri, DEFAULT_ROOT_DIR);

    if(fullPath == NULL)
        goto close_conn;

    INFO_LOG("[error] Serving static file: %s", fullPath);

    char *content = 0;

    int result = get_content_from_file(fullPath, &content);

    char* extension = get_extension(fullPath);

    if (!result)
    {
        INFO_LOG("[error] Couldn't find entry %s", fullPath);

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
            ERROR_LOG("[error] Cannot read file %s", strerror(errno));
        #endif
    }

    write_ln_to_socket(*client_socket_id, request_to_send); // Request line

    write_content_to_socket(*client_socket_id, content, get_mime_from_type(extension), 0);

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

    INFO_LOG("[server] Starting webserver on port %d", port);

    thread_pool_t pool;

    if(!initialize_threadpool(&pool, 8)) 
    {
        ERROR_LOG("[error] Failed to initialize thread pool %s", strerror(errno));
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