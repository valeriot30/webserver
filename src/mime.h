/**
 * @file mime.h
 * @author Valerio Triolo
 * @brief 
 * @version 0.1
 * @date 2024-04-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MIME_H
#define MIME_H

#include <string.h>

#define MIME_TYPE_HTML "text/html"

#define MIME_TYPE_DEFAULT MIME_TYPE_HTML

#define MIME_TYPE_CSS "text/css"
#define MIME_TYPE_JSON "application/json"
#define MIME_TYPE_PNG "image/png"


/**
 * @brief Get the mime type from file extension (also known as file type)
 * The mime type return should packed inside the response
 *  
 * @param extension 
 * @return char* 
 */
char* get_mime_from_type(char* extension);

#endif