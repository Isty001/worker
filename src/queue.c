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
    uint16_t count;
    pthread_mutex_t mutex;
};


Queue *queue_new(void)
{
    Queue *queue = malloc(sizeof(Queue));
    queue->head = NULL;
    queue->last = NULL;
    queue->count = 0;
    pthread_check(pthread_mutex_init(&queue->mutex, NULL));

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
    pthread_check(pthread_mutex_destroy(&queue->mutex));

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
    queue->count++;

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
    queue->count--;

    mutex_unlock(queue);

    void *message = tmp->message;
    free(tmp);

    return message;
}

uint16_t queue_count(Queue *queue)
{
    mutex_lock(queue);
    uint16_t count = queue->count;
    mutex_unlock(queue);

    return count;
}
