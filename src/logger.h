#ifndef LOG_H
#define LOG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static inline char *timenow();

#define _FILE strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__

#define LOG_ARGS(LOG_TAG)   timenow(), LOG_TAG, _FILE, __LINE__

#define LOG_FMT  "[%s] [%s] [%s:%d] "

#define NEWLINE     "\n"

#ifdef __OBJC__
#define PRINTFUNCTION(msg, ...)      objc_print(@msg, __VA_ARGS__)
#else
#define PRINTFUNCTION(msg, ...)      fprintf(stderr, msg, __VA_ARGS__)

#endif

#define ERROR_TAG "ERROR"
#define INFO_TAG "INFO"

#define INFO_LOG(msg, args...) PRINTFUNCTION(LOG_FMT msg NEWLINE, LOG_ARGS(INFO_TAG), ##args)

#define ERROR_LOG(msg, args...) PRINTFUNCTION(LOG_FMT msg NEWLINE, LOG_ARGS(ERROR_TAG), ##args)



static inline char *timenow() {
    static char buffer[64];
    time_t rawtime;
    struct tm *timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", timeinfo);
    
    return buffer;
}


#endif