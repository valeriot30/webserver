/*
 * *********************************************************
 * 
 * File: server.c
 * File Created: Saturday, 13th April 2024 3:38:16 pm
 * Author: Valerio Triolo
 * *********************************************************
 * Last Modified: Friday, 24th May 2024 10:43:50 am
 * Modified By: Valerio Triolo
 * 
 * *********************************************************
 */


#include "server.h"

sem_t resources;

#ifdef __APPLE__
    sem_t resources = 1;
#endif

/**
 * @brief Write a response_t object to socket based on sockfd
 * 
 * @param sockfd 
 * @param response 
 */
void write_response_to_socket(int sockfd, response_t *response)
{
    char *content_length_str = get_content_length_str(response);

    char* response_line = get_response_line(response);
    
    char* content_server = str_safe_concat(SERVER_PRODUCT_NAME, get_hostname_os()); 
    char* content_type = get_response_mime_str(response);
    char* content = get_content_str(response);

    int content_length = get_content_length(response);

    write_ln_to_socket(sockfd, response_line, strlen(response_line)); // Request line

    write_ln_to_socket(sockfd, content_server, strlen(content_server));
    write_ln_to_socket(sockfd, content_type, strlen(content_type));
    write_ln_to_socket(sockfd, content_length_str, strlen(content_length_str));
    
    if(!is_response_text(response)) {
        write_ln_to_socket(sockfd, RESPONSE_ACCEPT_RANGE, strlen(RESPONSE_ACCEPT_RANGE));
    }

    write_ln_to_socket(sockfd, "", 0);
    write_ln_to_socket(sockfd, content, content_length);

    free(content_length_str);
    free(content_server);
    free(content_type);
}


/**
 * TCP is a byte-stream protocol, so sending data individually is not affecting how the client (browser) is gonna receive the informations.
 * Using only one big message is a waste of memory somehow.
 */
void write_ln_to_socket(int sockfd, char *message, int len)
{   
    send(sockfd, message, len, 0);
    send(sockfd, "\r\n", 2, 0); // CRLF
}

inline static char* decode_path(char* request, uri_t* uri) {
    if(uri == NULL) {
        ERROR_LOG("[error] Error decoding uri %s", strerror(errno));
        return NULL;
    }

    char *fullPath = get_full_path(uri, DEFAULT_ROOT_DIR);

    INFO_LOG("[error] Serving static file: %s", fullPath);

    return fullPath;
}

static char* process_content(char* fullPath) {
    char *content = 0;

    // TODO can improve this by using only 1 pointer
    int result = get_content_from_file(fullPath, &content);
    char* extension = get_extension(fullPath);

    if(strcmp(extension, ".php") == 0) {
        // php code detected

        char* result = execute_php_script("-f", fullPath);

        if(result != NULL)
            strcpy(content, result);
    }

    if (!result)
    {
        INFO_LOG("[error] Couldn't find entry %s", fullPath);

        if (strcmp(fullPath, "public/index.html") == 0)
        {
            // explore
            // get_content_dir(".");
            content = "<html>List of all directories</html>";
        }
        else
        {
            content = "<html>404 error</html>";
            return NULL;
        }
    }
    else {
        #ifdef FILE_DEBUG 
            ERROR_LOG("[error] Cannot read file %s", strerror(errno));
        #endif
    }

    // probably this will be malloc'ed
    return content;
    
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

    char *fullPath = decode_path(request, uri);

    if(fullPath == NULL)
        goto close_conn;

    char *content = process_content(fullPath);

    if(content == NULL) request_to_send = HTTP_RESPONSE_404;

    response_t *generated_response;

    char* extension = get_extension(fullPath);

    char* mime_type = get_mime_from_type(extension);
    bool is_text = is_mime_text(mime_type);
    
    //printf("is_text = %d" , is_text);
    int content_length = is_text ? strlen(content) : get_file_size(fullPath);

    if(alloc_response(&generated_response, content, mime_type, is_text, content_length, request_to_send) == -1) {
         ERROR_LOG("[error] Cannot compose a response %s", strerror(errno));
         goto close_conn;
    }
 
    write_response_to_socket(*client_socket_id, generated_response);

    goto close_conn;

    close_conn:
        // we flush buffer
        free(fullPath);
        free_response(generated_response);
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

        // todo replace with pool->assign(task)
        pthread_create(&id, NULL, new_client_instance, (void*) &new_socket);
        pthread_join(id, NULL);

        sem_post(&resources);
    }

    destroy_threadpool(&pool);

    close(sd);

    return sd;
}
