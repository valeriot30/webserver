#ifndef THREAD_H
#define THREAD_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct worker {
    pthread_t tid;
} worker_t;

typedef struct task {
    void (*return_value)(void*);
} task_t;

typedef struct thread_pool {

} thread_pool_t;

#endif