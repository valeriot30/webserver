#include "net.h"

volatile sig_atomic_t sd = 0;

inline static void socket_term(int signum)
{
    close(sd);
}
int create_client_socket(unsigned int port)
{
    struct sockaddr_in client_address;
    
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = socket_term;
    sigaction(SIGTERM, &action, NULL);

    int namelen = sizeof(client_address);

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_address.sin_addr), clientIP, INET_ADDRSTRLEN);

    int new_socket = accept(sd, (struct sockaddr *)&client_address, (socklen_t *)&namelen);

    if (new_socket == -1)
    {
        ERROR_LOG("Error accepting socket on port %d [%s]\n", port, strerror(errno));
        return -1;
    }

    INFO_LOG("Started request connection on %d by %s", port, clientIP);

    return new_socket;
}
int create_server_socket(unsigned int port)
{
    struct sockaddr_in server_address;

    sd = socket(AF_INET, SOCK_STREAM, 0);

    // let's catch sigterm for preventing I/O Blocking
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = socket_term;
    sigaction(SIGTERM, &action, NULL);
    // signal(SIGINT, socket_term);

    if (sd == -1)
    {
        ERROR_LOG("Error creating socket");
        goto error;
    }

    int b = 1;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int sock_enable_port_reuse = 1;

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &sock_enable_port_reuse, sizeof(int)) < 0)
        ERROR_LOG("setsockopt(SO_REUSEADDR) failed");

    b = bind(sd, (struct sockaddr *)&server_address, sizeof(server_address));

    if (b == -1)
    {
        ERROR_LOG("Error binding socket on port %d [%s]\n", port, strerror(errno));
        goto error;
    }

    if (listen(sd, 1) != 0)
    {
        fprintf(stderr, "Error listening socket on port %d [%s]\n", port, strerror(errno));
        goto error;
    }

    error:
        return -1;

    return sd;
}