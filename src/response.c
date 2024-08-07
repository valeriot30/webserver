#include "response.h"


int alloc_response(response_t** response, char* content, char* mime_type, bool is_text, int content_length, char* response_line) {

    *response = (response_t*) malloc(sizeof(struct response));

    if(response == NULL) {
        return -1;
    }

    (*response)->content_length = content_length;
    (*response)->is_text = is_text;
    (*response)->content = content;
    (*response)->mime_type = mime_type;
    (*response)->response_line = response_line;

    return 1;
}

int get_content_length(response_t* response) {
    return response->content_length;
}

char* get_content_length_str(response_t* response) {
    char length_str[262144];
    memset(length_str, 0, sizeof length_str);
    sprintf(length_str, "%d", (int)response->content_length);

    return str_safe_concat(RESPONSE_CONTENT_LENGTH, length_str);
}

char* get_response_line(response_t* response) {
    return response->response_line;
}

bool is_response_text(response_t* response) {
    return response->is_text;
}

char* get_content_str(response_t* response) {
    return response->content;
}

char* get_response_mime_str(response_t* response) {
    return str_safe_concat(RESPONSE_CONTENT_TYPE, response->mime_type);
}

char* get_response_mime_type(response_t* response) {
    return response->mime_type;
}

int free_response(response_t* response) {
    if(response == NULL) return -1;

    free(response);
        
    return 1;
}