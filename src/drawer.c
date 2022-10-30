#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../header/drawer.h"

const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";

/* Initializes a new Drawer
 * The timestamps and initial delay are initialized to 0
 * Allocates space for the thread_id double pointer of the Drawer, which should be shared with the KeyListener
 * Return: Pointer to the initialized Drawer
 */
Drawer* init_drawer() {
    Drawer *new = malloc(sizeof(Drawer));
    new->ld_sec = 0;
    new->ld_msec = 0;
    new->update_delay_s = 0;
    new->update_delay_ms = 0;
    new->display = init_display();

    new->thread_id = NULL;

    return new;
}

/* Deletes a Drawer from memory, including its Display and the thread_id (if allocated)
 */
void delete_drawer(Drawer *drawer) {
    delete_display(drawer->display);
    if (drawer->thread_id != NULL) {
        free(drawer->thread_id);
    }
    free(drawer);
    clear_screen();
}

/* Draws the display on the screen
 * Assumes setFPS has been called
 * Waits the preset amount of time so the framerate can be equal to the preset
 */
void drawer_draw_display(Drawer *drawer) {
    time_t sec;
    long msec;
    get_timestamp(&sec, &msec);

    while (sec - drawer->ld_sec <= drawer->update_delay_s && msec - drawer->ld_msec <= drawer->update_delay_ms) {
        usleep(10000);
        get_timestamp(&sec, &msec);
    }

    ulong total_to_write = drawer->display->_rows * drawer->display->_columns * CELLBYTES * sizeof(char);
    ssize_t written = 0;
    ssize_t count;
    do {
        count = write(STDOUT_FILENO, &drawer->display->_display_array[written], total_to_write - written);
        if (count != -1) {
            written += count;
        }
    } while (written != total_to_write);
    fflush(stdout);

    get_timestamp(&drawer->ld_sec, &drawer->ld_msec);
}

/* Determines the framerate the game will run at
 * This function must be called at least once before drawer_draw_display to initialize the FPS value
 */
int drawer_set_fps(Drawer *drawer, int val) {
     if (val <= 0) {
         return 1;
     }

     drawer->update_delay_s = (int)(1.0 / val);
     drawer->update_delay_ms = (long)((1.0 / val) * 1000);
     return 0;
}

/* Starts the drawer thread, which runs the game loop function passed as argument f
 * Additionally, accepts the Drawer and the shared Queue
 */
void drawer_start_thread(Drawer *drawer, Queue *queue, void *(*f)(void *args)) {
    GameloopFuncArgs args;
    args.drawer = drawer;
    args.queue = queue;

    drawer->thread_id = malloc(sizeof(pthread_t));
    pthread_create(drawer->thread_id, NULL, f, &args);
}

// Utility functions
// The below functions are to be called by the game loop function to get the Drawer and Queue
// They can be replaced by casting (void *args) to (GameloopFuncArgs *) and getting the Drawer and Queue from it

/* Accepts the (void *args) argument passed to the game loop function by drawer_start_thread and returns the Drawer
 */
Drawer* args_get_drawer(void *args) {
    return ((GameloopFuncArgs *)args)->drawer;
}

/* Accepts the (void *args) argument passed to the game loop function by drawer_start_thread and returns the Queue
 */
Queue* args_get_queue(void *args) {
    return ((GameloopFuncArgs *)args)->queue;
}

/* Clears the terminal screen
 */
void clear_screen() {
    system("clear");
    //write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}