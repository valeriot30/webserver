
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

void process_config_descriptor(t_config* configfd, char* key, char* value) {
    if(strcmp(CONFIG_PORT, key) == 0) {
        (configfd)->port = atoi(value);
    }
    else if(strcmp(CONFIG_HOST, key) == 0) {
        (configfd)->host = value;
    }
    else if(strcmp(CONFIG_ROOT_DIR, key) == 0) {
        (configfd)->root_dir = value;
    }
}

void parse_config(const char* filename, t_config* configfd ) 
{
    FILE *file = fopen(filename, "r");

    if(configfd == NULL) {
        fprintf(stderr, "Cannot allocate config descriptor for server %s", strerror(errno));
        exit(EXIT_FAILURE);
        goto close_file;
    }

    if (file == NULL) {
        fprintf(stderr, "Error opening file");
        goto close_file;
    }

    char line[MAX_LINE_LENGTH];

    // Flag to indicate when we are inside the webserver object
    int inside = 0;

    while (fgets(line, sizeof(line), file) != NULL) 
    {
        size_t len = strlen(line);

        // Remove newline character at the end of the line
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Check if we are inside the webserver object
        if (strstr(line, "webserver: {") != NULL) {
            inside = 1;
            if(DEV_DEBUG)
                printf("[config] Found webserver object\n");

            continue;
        } else if (inside && strstr(line, "}") != NULL) {
            inside = 0;
            if(DEV_DEBUG)
                printf("[config] End of webserver object\n");

            continue;
        }

        // Parse key and value when inside the webserver object
        if (inside) {
            char *key = strtok(line, ":");
            char *value = strtok(NULL, ":");

            // Remove leading and trailing whitespaces from key and value
            if (key != NULL && value != NULL) {
                printf("[config] Key: %s, Value: %s\n", rtrim(key), rtrim(value));
                process_config_descriptor(configfd, key, value);
            }
        }
    }

    goto close_file;

    close_file:
        fclose(file);
        return;
}

void* get_config_value(t_config* configfd, const char* config_key){
    if(strcmp(CONFIG_PORT, config_key) == 0) {
        return &(configfd->port);
    }
    else if(strcmp(CONFIG_HOST, config_key) == 0) {
        return configfd->host;
    }
    else if(strcmp(CONFIG_ROOT_DIR, config_key) == 0) {
        return configfd->root_dir;
    }

    return NULL;
}

int allocate_config_descriptor(t_config* configfd) {
    FILE *file = fopen(FILECONFIG_NAME, "r");

    if (file == NULL) {
        #ifdef DEV_DEBUG
            fprintf(stderr, "Error opening config file\n");
        #endif
        fclose(file);
        return -1;
    }
    parse_config(FILECONFIG_NAME, configfd);

    fclose(file);

    return 1;
}
