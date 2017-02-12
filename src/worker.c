#include <malloc.h>
#include <stdbool.h>
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
    void *msg;

    while (worker->alive) {
        if ((msg = queue_remove(worker->queue))) {

            if (0 != (err = worker->execute(msg))) {
                worker->error(err);
            }
        } else {
            nanosleep(&worker->timeout, NULL);
        }
    }

    return NULL;
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

void worker_run(Worker *worker)
{
    pthread_check(pthread_create(&worker->thread, NULL, (void *(*)(void *)) execute, worker));
}

void worker_kill(Worker *worker)
{
    worker->alive = false;
    pthread_join(worker->thread, NULL);
}

void worker_free(Worker *worker)
{
    worker_kill(worker);
    free(worker);
}
