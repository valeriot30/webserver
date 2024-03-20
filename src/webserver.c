#include <stdio.h>
#include <stdlib.h>
#include "server.h"

int main(int argv, char** argc) {

    t_config configfd = {0};

    int result = allocate_config_descriptor(&configfd);

    if(result == -1) {
         fprintf(stderr, "Error allocating configuration resource [%s]\n", strerror(errno));
         return -1;
    }

    int sd = create_http_server(configfd);

    if(sd == -1) {
        fprintf(stderr, "Error creating http server %s\n", strerror(errno));
        return -1;
    }

    return 0;
}