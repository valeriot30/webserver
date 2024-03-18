
#include "buffer.h"

int allocate_buffer(int len, buffer_t* buf) {
    buf = (struct buffer*) malloc(sizeof(struct buffer));

    if(buf == NULL) {
        fprintf(stdout, "Could not allocate buffer");
        return -1;
    }

    buf->content = malloc(len);

    return 0;
    
}

int flush_content_from_buffer(char* buffer) {
    if(buffer != NULL) {
        free(buffer);
        return 0;
    }

    return -1;
}

int get_content_from_file(char* filename, char**buffer) {
    long length;

    FILE * f = fopen (filename, "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        *buffer = malloc (length);
        if (*buffer)
        {
            fread (*buffer, 1, length, f);
        }
        fclose (f);

        return 1;
        
    }
    
    return 0;
}
