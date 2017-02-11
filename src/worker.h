#ifndef ROGUE_CRAFT_WORKER_H
#define ROGUE_CRAFT_WORKER_H


#include "stdint.h"

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
