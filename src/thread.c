/**
 * @file thread.c
 * 
 *  In computer programming, a thread pool is a software design pattern for achieving concurrency of execution in a computer program. 
 * Often also called a replicated workers or worker-crew model, 
 * a thread pool maintains multiple threads waiting for tasks to be allocated for concurrent execution by the supervising program. 
 * By maintaining a pool of threads, the model increases performance and avoids latency in execution due to frequent creation and destruction of threads for short-lived tasks. 
 * The number of available threads is tuned to the computing resources available to the program, such as a parallel task queue after completion of execution.
 * 

 * @author Valerio Triolo
 * @brief 
 * @version 0.1
 * @date 2024-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "thread.h"

static int destroy_worker(worker_t* worker) {
    if(worker == NULL) return -1;

    pthread_exit(NULL);

    free(worker);

    return 1;
}

static worker_t* initialize_worker() {
    worker_t* worker = (worker_t*) malloc(sizeof(struct worker));

    if(worker == NULL)
        return NULL;

    return worker;
}

bool initialize_threadpool(thread_pool_t* instance, unsigned int num_threads) {
    if(num_threads == 0) 
    {
         fprintf(stderr, "Too few num_threads given");
         return false;
    }

    // we check for num_thread to be a power of 2
    if((num_threads & (num_threads - 1)) != 0) 
    {
        fprintf(stderr, "[warning] num_threads should a power of two");
    }

    //TODO calculate max threads in order to optimize performance

    instance = (thread_pool_t*) malloc(sizeof(struct thread_pool));

    if(instance == NULL) {
         fprintf(stderr, "error allocating thread pool");
         return false;
    }

    if ((pthread_mutex_init(&instance->lock, NULL)) != 0) {
        fprintf(stderr, "Error initializing lock.\n");
        return false;
    }

    instance->num_works = num_threads;

    instance->workers = (worker_t**) calloc(num_threads, sizeof(struct worker));    

    for(int i = 0; i < num_threads; i++){
        instance->workers[i] = initialize_worker();
    }

    return true;

}

int get_pool_works(thread_pool_t* instance) {
    if(instance == NULL) {
        return 0;
    }

    return instance->num_works;
}

int destroy_threadpool(thread_pool_t* instance) {
    if(instance == NULL) {
        return -1;
    }

    for (int i = 0; i < instance->num_works; i++)
    {
        destroy_worker(instance->workers[i]);
    }

    return 1;
}