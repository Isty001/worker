#ifndef WORKER_H
#define WORKER_H


#include <stdint.h>
#include <stdlib.h>


#define pthread_check(res)                              \
    if (0 != res) {                             \
        fprintf(stderr, "pthread failure");     \
        exit(EXIT_FAILURE);                     \
    }

#define mutex_lock(s)                                     \
    pthread_check(pthread_mutex_lock(&s->mutex));

#define mutex_unlock(s)                                        \
    pthread_check(pthread_cond_broadcast(&s->cond));     \
    pthread_check(pthread_mutex_unlock(&s->mutex));


typedef struct Queue Queue;

typedef struct Worker Worker;

typedef int (*Executor)(void *);

typedef void (*ErrorHandler)(int);


Queue *queue_new(void);

void queue_add(Queue *queue, void *message);

void *queue_remove(Queue *queue);

uint16_t queue_count(Queue *queue);

void queue_free(Queue *queue);

Worker *worker_new(Queue *queue, Executor executor, ErrorHandler handler, uint16_t sleep_ms);

void worker_run(Worker *worker);

void worker_kill(Worker *worker);

void worker_free(Worker *worker);


#endif
