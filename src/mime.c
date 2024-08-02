/*
 * *********************************************************
 * 
 * @File: mime.c
 * @File Created: Wednesday, 10th April 2024 10:27:39 pm
 * @Author: Valerio Triolo
 * *********************************************************
 */


#include "mime.h"

char* get_mime_from_type(char* extension) {

    if(extension == NULL) {
        return MIME_TYPE_DEFAULT;
    }

    // O(n)
    if(strcmp(extension, ".html") == 0) {
            return MIME_TYPE_HTML;
    }
    else if(strcmp(extension, ".css") == 0) {
            return MIME_TYPE_CSS;
    }
    else if(strcmp(extension, ".png") == 0) {
            return MIME_TYPE_PNG;
    }
    else if(strcmp(extension, ".ico") == 0) {
        return MIME_TYPE_ICO;
    }
    else if(strcmp(extension, ".json") == 0) {
        return MIME_TYPE_JSON;
    }
    else if(strcmp(extension, ".webp") == 0) {
        return MIME_TYPE_WEBP;
    } else {
        return MIME_TYPE_HTML;
    }

    return MIME_TYPE_DEFAULT;
}

bool is_mime_text(char * mime_type) {
    return strcmp(mime_type, MIME_TYPE_HTML) == 0 || strcmp(mime_type, MIME_TYPE_JSON) == 0;
}