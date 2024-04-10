#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

#define DEV_DEBUG 1

#define DEV_CONFIG_FILE_DEBUG 0

#define FILE_DEBUG 0

#define MAX_LINE_LENGTH 256

#define FILECONFIG_NAME "server.conf"

// all keywords
static const char* CONFIG_HOST = "host"; 
static const char* CONFIG_PORT = "port";
static const char* CONFIG_ROOT_DIR = "root";

typedef struct config_descriptor {
    unsigned int port;
    char* host;
    char* root_dir;
} t_config;

int allocate_config_descriptor(t_config* configfd);
void* get_config_value(t_config* configfd, const char* config_key);
#endif