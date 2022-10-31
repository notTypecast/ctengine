#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "../header/queue.h"

/* Initializes an empty Queue
 * The timestamp value of the last put operation is initialized to 0
 * Return: Pointer to the initialized Queue
 */
Queue* init_queue() {
    Queue *new = malloc(sizeof(Queue));
    new->head = NULL;
    new->tail = NULL;
    new->lpt_sec = 0;
    new->lpt_ms = 0;
    if (pthread_mutex_init(&new->mutex, NULL) != 0) {
        printf("Could not initialize queue mutex");
        return NULL;
    }
    new->finished = 0;
    return new;
}

/* Deletes an existing Queue, as well as the items contained within it
 * NOT THREAD SAFE: only the calling thread should be using the queue if this function is called
 * The function makes an attempt at mutual exclusion while it deletes QueueItems, but the mutex must be unlocked before
 *  it is destroyed; if after being unlocked, another thread accesses the Queue and begins an operation on it, then the
 *  Queue is destroyed by this thread before that operation finishes, it could cause unexpected results
 */
void delete_queue(Queue *queue) {
    pthread_mutex_lock(&queue->mutex);
    QueueItem *curr = queue->head, *tmp;
    while (curr != NULL) {
        tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    pthread_mutex_unlock(&queue->mutex);
    pthread_mutex_destroy(&queue->mutex);
    free(queue);
}

/* Checks for emptiness of Queue
 * THREAD SAFE
 * Return: 1 if Queue is empty, else 0
 */
int queue_empty(Queue *queue) {
    pthread_mutex_lock(&queue->mutex);
    int empty = (queue->head == NULL);
    pthread_mutex_unlock(&queue->mutex);
    return empty;
}

/* Adds a new value to the end of the Queue
 * THREAD SAFE
 * If the Queue contained no items, the newly added item will be both head and tail
 */
void queue_put(Queue *queue, int val) {
    // create new QueueItem
    QueueItem *new = malloc(sizeof(QueueItem));
    new->val = val;
    new->next = NULL;
    pthread_mutex_lock(&queue->mutex);
    // if queue is empty, add item as both head and tail
    if (queue->head == NULL) {
        queue->head = new;
        queue->tail = new;
    }
    else {
        // make previous last item (tail) point to the new item
        queue->tail->next = new;
        // make the new item the last item (tail)
        queue->tail = new;
    }
    get_timestamp(&queue->lpt_sec, &queue->lpt_ms);
    pthread_mutex_unlock(&queue->mutex);
}

/* Gets and removes the next item from the Queue
 * THREAD SAFE
 * This function assumes the Queue is not empty
 * The removed item is also deleted from memory
 * Return: the value of the QueueItem
 */
int queue_get(Queue *queue) {
    pthread_mutex_lock(&queue->mutex);
    QueueItem *tmp = queue->head;
    int val = tmp->val;
    // make the next item be the head
    queue->head = tmp->next;
    // if there was no next item, make tail also be NULL
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    pthread_mutex_unlock(&queue->mutex);
    free(tmp);
    return val;
}

/* Clears the Queue of all items contained within it
 * Deletes QueueItems contained in Queue from memory
 * THREAD SAFE
 */
void queue_clear(Queue *queue) {
    pthread_mutex_lock(&queue->mutex);
    QueueItem *curr = queue->head, *tmp;
    while (curr != NULL) {
        tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    queue->head = NULL;
    queue->tail = NULL;
    pthread_mutex_unlock(&queue->mutex);
}

/* Gets the current UNIX timestamp
 * Saves the seconds in sec and the remaining milliseconds in msec
 */
void get_timestamp(time_t *sec, long *msec) {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);

    *sec = spec.tv_sec;
    *msec = (long)round((double)spec.tv_nsec / 1e6);

    if (*msec > 999) {
        ++*sec;
        *msec = 0;
    }
}



