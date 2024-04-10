#include "mime.h"

char* get_mime_from_type(char* extension) {

    if(extension == NULL) {
        return MIME_TYPE_DEFAULT;
    }

    // O(n)
    if(strcmp(extension, ".html")) {
            return MIME_TYPE_HTML;
    }
    else if(strcmp(extension, ".css")) {
            return MIME_TYPE_CSS;
    }
    else if(strcmp(extension, ".png")) {
            return MIME_TYPE_PNG;
    }
    else if(strcmp(extension, ".json")) {
        return MIME_TYPE_JSON;
    }

    return MIME_TYPE_DEFAULT;
}