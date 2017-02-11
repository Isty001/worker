#include <malloc.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include "worker.h"


struct Worker {
    Queue *queue;
    Executor execute;
    ErrorHandler error;
    bool alive;
    struct timespec timeout;
    pthread_t thread;
};

static void *execute(Worker *worker)
{
    int err;

    while (worker->alive) {
        if (queue_count(worker->queue)) {
            err = worker->execute(queue_remove(worker->queue));

            if (0 != err) {
                worker->error(err);
            }
        }
        nanosleep(&worker->timeout, NULL);
    }

    return NULL;
}

void worker_run(Worker *worker)
{
    pthread_create(&worker->thread, NULL, (void *(*)(void *)) execute, worker);
}

void worker_kill(Worker *worker)
{
    worker->alive = false;
    pthread_join(worker->thread, NULL);
}

Worker *worker_new(Queue *queue, Executor executor, ErrorHandler handler, uint16_t sleep_ms)
{
    Worker *worker = malloc(sizeof(Worker));
    worker->timeout.tv_nsec = sleep_ms * 1000000;
    worker->timeout.tv_sec = 0;
    worker->queue = queue;
    worker->execute = executor;
    worker->error = handler;
    worker->alive = true;

    return worker;
}

void worker_free(Worker *worker)
{
    worker_kill(worker);
    free(worker);
}
