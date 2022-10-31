#ifndef TENGINE_DRAWER_H
#define TENGINE_DRAWER_H

#include <pthread.h>
#include "display.h"
#include "queue.h"

// value to clear terminal screen
extern const char *CLEAR_SCREEN_ANSI;

/* Defines a Drawer
 * time_t ld_sec: UNIX timestamp of last draw onto screen in seconds
 * long ld_msec: the millisecond part of the ld_sec timestamp
 * long update_delay_us: delay between each screen update in microseconds
 * Display *display: the Display used to represent the screen
 * pthread_t **thread_id: pointer to a pointer to the id value of the thread to run the drawer
 *  double pointer is required because the double pointer itself must be shared with the KeyListener, but must also be
 *  able to point to NULL initially (before the drawer thread is started); once the drawer thread is started, the double
 *  pointer is changed to point to a pointer, which points to the pthread_t value of the thread; this allows the
 *  KeyListener to know if the thread was started and free the pointer if necessary on exit, as well as warn in case the
 *  KeyListener was started without the drawer thread
 * char *exit_msg: message to be displayed on exit; set by drawer_set_exit_msg
 */
typedef struct {
    time_t ld_sec;
    long ld_msec;
    int update_delay_s;
    long update_delay_ms;
    Display *display;
    pthread_t *thread_id;
    char *exit_msg;
} Drawer;

/* Defines a GameloopFuncArgs struct, used to pass the required arguments to the game loop function
 * Drawer *drawer: Pointer to the Drawer
 * Queue *queue: Pointer to the shared Queue
 */
typedef struct {
    Drawer *drawer;
    Queue *queue;
} GameloopFuncArgs;

// Drawer functions
Drawer* init_drawer();
void delete_drawer(Drawer *drawer);
void drawer_draw_display(Drawer *drawer);
int drawer_set_fps(Drawer *drawer, int val);
void drawer_start_thread(Drawer *drawer, Queue *queue, void *(*f)(void *args));
void drawer_set_exit_msg(Drawer *drawer, const char* msg);
void drawer_clear_exit_msg(Drawer *drawer);

// Utility functions
Drawer* args_get_drawer(void *args);
Queue* args_get_queue(void *args);
void clear_screen();

#endif //TENGINE_DRAWER_H
