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
    else if(strcmp(extension, ".json") == 0) {
        return MIME_TYPE_JSON;
    }

    return MIME_TYPE_DEFAULT;
}