#ifndef TENGINE_QUEUE_H
#define TENGINE_QUEUE_H

#include <time.h>

/* Defines a queue item containing an integer
 * int val: the value contained in the item
 * struct QueueItem *next: pointer to the item below this one in the queue
 */
typedef struct QueueItem {
    int val;
    struct QueueItem *next;
} QueueItem;

/* Defines a thread-safe queue of QueueItems
 * QueueItem *head: the QueueItem which will be removed on the next get operation
 * QueueItem *tail: the QueueItem which was last added to the Queue
 * time_t lpt_sec: UNIX timestamp of last put operation called on the queue, in seconds
 * long lpt_ms: the millisecond part of the lpt_sec timestamp
 * pthread_mutex_t mutex: the mutex used to ensure mutual exclusion when queue is used with multiple threads
 */
typedef struct {
    QueueItem *head;
    QueueItem *tail;
    time_t lpt_sec;
    long lpt_ms;
    pthread_mutex_t mutex;
} Queue;

// Queue operations
Queue* init_queue();
void delete_queue(Queue *queue);
int queue_empty(Queue *queue);
void queue_put(Queue *queue, int val);
int queue_get(Queue *queue);
void queue_clear(Queue *queue);

// Utility functions
void get_timestamp(time_t *sec, long *msec);

#endif //TENGINE_QUEUE_H
