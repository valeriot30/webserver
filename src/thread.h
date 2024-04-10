#ifndef THREAD_H
#define THREAD_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#define THREAD_STACK 1024*1024 // we assume thread stack will take 1 mb of memory

typedef struct worker {
    pthread_t tid;
    bool running;
} worker_t;

typedef struct task {
    void (*return_value)(void*);
} task_t;

typedef struct thread_pool {
    int num_works;
    worker_t** workers; // the handle of the worker list
    pthread_mutex_t lock;
} thread_pool_t;

/**
 * @brief Create the thread pool by defining num_threads
 * 
 * 
 * @param instance the thread pool instance
 * @param num_threads the number of threads (workers) kept in reserve for executing tasks
 * @return bool
 */
bool initialize_threadpool(thread_pool_t* instance, unsigned int num_threads);

/**
 * @brief Get the pool number of threads working at it.
 * 
 * @param instance 
 * @return int the num of workers currently available on pool
 */
int get_pool_works(thread_pool_t* instance);

/**
 * @brief Destroy all the thread pool
 * 
 * @param instance 
 * @return true if 
 */
int destroy_threadpool(thread_pool_t* instance);

#endif