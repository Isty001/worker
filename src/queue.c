#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include "worker.h"


#define check(res)                              \
    if (0 != res) {                             \
        fprintf(stderr, "pthread failure");     \
        exit(EXIT_FAILURE);                     \
    }

#define lock(queue) \
    check(pthread_mutex_lock(&queue->mutex));

#define unlock(queue)                                \
    check(pthread_cond_broadcast(&queue->cond));     \
    check(pthread_mutex_unlock(&queue->mutex));


typedef struct Node {
    void *message;
    struct Node *next;
} Node;

struct Queue {
    Node *head;
    Node *last;
    uint16_t count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};


Queue *queue_new(void)
{
    Queue *queue = malloc(sizeof(Queue));
    queue->head = NULL;
    queue->last = NULL;
    queue->count = 0;
    check(pthread_mutex_init(&queue->mutex, NULL));
    check(pthread_cond_init(&queue->cond, NULL));

    return queue;
}

void queue_free(Queue *queue)
{
    Node *tmp, *head = queue->head;

    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void queue_add(Queue *queue, void *message)
{
    Node *new = malloc(sizeof(Node));
    new->message = message;
    new->next = NULL;

    lock(queue);

    if (queue->last) {
        queue->last->next = new;
    } else {
        queue->head = new;
    }
    queue->last = new;
    queue->count++;

    unlock(queue);
}

void *queue_remove(Queue *queue)
{
    if (!queue->head) {
        return NULL;
    }

    lock(queue);

    Node *tmp = queue->head;
    queue->head = tmp->next;

    if (!queue->head) {
        queue->last = NULL;
    }
    queue->count--;

    unlock(queue);

    void *message = tmp->message;
    free(tmp);

    return message;
}

uint16_t queue_count(Queue *queue)
{
    lock(queue);
    uint16_t count = queue->count;
    unlock(queue);

    return count;
}
