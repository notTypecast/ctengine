#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "../header/keylistener.h"


/* Initializes a KeyListener
 * Accepts a Queue as an argument, into which values corresponding to specific key presses will be placed
 * Accepts the Drawer which will run alongside this listener; as such, the Drawer must be created before the KeyListener
 * This function will automatically change the terminal flags and disable canonical mode, enabling the key listener to
 *  listen for key events
 * For this to be disabled, so the terminal can return to its original flags, delete_keylistener must be called
 */
KeyListener* init_keylistener(Queue *queue, Drawer *drawer) {
    KeyListener *new = malloc(sizeof(KeyListener));

    new->fd = STDIN_FILENO;

    tcgetattr(new->fd, &new->oldterm);
    memcpy(&new->newterm, &new->oldterm, sizeof(struct termios));
    new->newterm.c_lflag = new->newterm.c_lflag & !ICANON & !ECHO;
    tcsetattr(new->fd, TCSANOW, &new->newterm);

    int oldflags = fcntl(new->fd, F_GETFL);
    fcntl(new->fd, F_SETFL, oldflags | O_NONBLOCK);

    new->eQueue = queue;
    new->rec_keycodes = init_keymap();
    new->drawer = drawer;

    return new;
}

/* Deletes a KeyListener, including its internal KeyMap and all the values contained within it
 * Does not delete Queue, since it is shared and must be deleted separately; places value 0 in Queue instead, so that
 *  the drawer thread knows to quit once it is read
 * Returns terminal to original state
 */
void delete_keylistener(KeyListener *key_listener) {
    tcsetattr(key_listener->fd, TCSAFLUSH, &key_listener->oldterm);
    fcntl(key_listener->fd, F_SETFL, key_listener->oldflags);
    delete_keymap(key_listener->rec_keycodes);
    queue_put(key_listener->eQueue, 0);
    free(key_listener);
}

/* Adds a key-int pair to the internal KeyMap of the KeyListener
 * Once keylistener_handle_in is called, any time the key corresponding to the key value passed is pressed, the value
 *  will be placed in the shared Queue to be read by the drawer thread
 */
void keylistener_add_key(KeyListener *key_listener, const char key[KEYSIZE], int val) {
    keymap_put(key_listener->rec_keycodes, key, val);
}

/* Handles input
 * This must be run on the main thread and blocks execution
 * Repeatedly reads input and, once a key contained in the internal KeyMap is read, places corresponding value in Queue
 * If key corresponding to value 0 is read, the value is placed in the Queue, after which the KeyListener waits for
 * the drawer thread to read the value 0 and exit and then exits itself
 */
void keylistener_handle_in(KeyListener *key_listener) {
    char c[KEYSIZE];
    for (int i = 0; i < KEYSIZE; ++i) {
        c[i] = 0;
    }
    while (1) {
        usleep(10000);
        int ecode = (int)read(key_listener->fd, c, KEYSIZE);

        if (ecode != -1) {
            //printf("0: [%d], 1: [%d], 2: [%d], 3: [%d], 4: [%d] %d\n", c[0], c[1], c[2], c[3], c[4], ecode);
            if (c[0] == 3) {
                keylistener_exit(key_listener);
                return;
            }
            if (keymap_has(key_listener->rec_keycodes, c)) {
                int val = keymap_get(key_listener->rec_keycodes, c);
                if (!val) {
                    keylistener_exit(key_listener);
                    return;
                }
                queue_put(key_listener->eQueue, val);
            }
            for (int i = 0; i < KEYSIZE; ++i) {
                c[i] = 0;
            }
        }
        else if (key_listener->eQueue->lpt_sec) {
            time_t sec;
            long msec;
            get_timestamp(&sec, &msec);
            if (sec > key_listener->eQueue->lpt_sec || msec - key_listener->eQueue->lpt_ms > 50) {
                queue_clear(key_listener->eQueue);
                key_listener->eQueue->lpt_sec = 0;
            }

        }
    }
}

/* Used by keylistener_handle_in to exit if an exit signal is read
 */
void keylistener_exit(KeyListener *key_listener) {
    Queue *queue = key_listener->eQueue;
    queue_put(queue, 0);
    Drawer *drawer = key_listener->drawer;
    delete_keylistener(key_listener);
    if (drawer->thread_id != NULL) {
        pthread_join(*drawer->thread_id, NULL);
    }
    else {
        printf("Warning: Key listener received exit signal, but drawer thread hasn't started\n");
        free(drawer->thread_id);
    }
    delete_drawer(drawer);
    delete_queue(queue);
}

const struct KEYCONST CONST = {
    "\x1b",
    "\x1bOP",
    "\x1bOQ",
    "\x1bOR",
    "\x1bOS",
    "\x1b[15~",
    "\x1b[17~",
    "\x1b[18~",
    "\x1b[19~",
    "\x1b[20~",
    "\x1b[21~",
    "\x1b[23~",
    "\x1b[24~",
    "`",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "~",
    "!",
    "@",
    "#",
    "$",
    "%",
    "^",
    "&",
    "*",
    "(",
    ")",
    "-",
    "_",
    "=",
    "+",
    "\x7f",
    "\t",
    "Q",
    "q",
    "W",
    "w",
    "E",
    "e",
    "R",
    "r",
    "T",
    "t",
    "Y",
    "y",
    "U",
    "u",
    "I",
    "i",
    "O",
    "o",
    "P",
    "p",
    "[",
    "{",
    "]",
    "}",
    "|",
    "\\",
    "A",
    "a",
    "S",
    "s",
    "D",
    "d",
    "F",
    "f",
    "G",
    "g",
    "H",
    "h",
    "J",
    "j",
    "K",
    "k",
    "L",
    "l",
    ";",
    ":",
    "'",
    "\"",
    "\n",
    "Z",
    "z",
    "X",
    "x",
    "C",
    "c",
    "V",
    "v",
    "B",
    "b",
    "N",
    "n",
    "M",
    "m",
    ",",
    "<",
    ".",
    ">",
    "/",
    "?",
    " ",
    "\x1b[2~",
    "\x1b[H",
    "\x1b[5~",
    "\x1b[3~",
    "\x1b[F",
    "\x1b[6~",
    "\x1b[A",
    "\x1b[D",
    "\x1b[B",
    "\x1b[C",
};
