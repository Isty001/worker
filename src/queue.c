#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include "worker.h"


typedef struct Node {
    void *message;
    struct Node *next;
} Node;

struct Queue {
    Node *head;
    Node *last;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};


Queue *queue_new(void)
{
    Queue *queue = malloc(sizeof(Queue));
    queue->head = NULL;
    queue->last = NULL;
    pthread_check(pthread_mutex_init(&queue->mutex, NULL));
    pthread_check(pthread_cond_init(&queue->cond, NULL));

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
    free(queue);
}

void queue_add(Queue *queue, void *message)
{
    Node *new = malloc(sizeof(Node));
    new->message = message;
    new->next = NULL;

    mutex_lock(queue);

    if (queue->last) {
        queue->last->next = new;
    } else {
        queue->head = new;
    }
    queue->last = new;

    mutex_unlock(queue);
}

void *queue_remove(Queue *queue)
{
    if (!queue->head) {
        return NULL;
    }

    mutex_lock(queue);

    Node *tmp = queue->head;
    queue->head = tmp->next;

    if (!queue->head) {
        queue->last = NULL;
    }

    mutex_unlock(queue);

    void *message = tmp->message;
    free(tmp);

    return message;
}
