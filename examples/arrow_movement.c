#include "../header/ctengine.h"
#include <stdio.h>
#include <unistd.h>

void* gameloop(void *args) {
    // get Drawer and Queue
    Drawer *drawer = args_get_drawer(args);
    Queue *queue = args_get_queue(args);

    // get terminal size
    int *size = display_get_size(drawer->display);

    // initialize player position and speed
    int player_pos[2] = {0, 0};
    int player_speed[2] = {0, 0};

    // UTF-8 character representing the player
    const char *square = "■";

    int running = 1;

    // main game loop
    while (1) {
        // handle key presses
        if (!queue_empty(queue)) {
            int val = queue_get(queue);

            switch (val) {
                // if 0 is read, exit
                case 0:
                    running = 0;
                    break;
                // 1 means UP ARROW
                case 1:
                    player_speed[0] = -1;
                    player_speed[1] = 0;
                    break;
                // 2 means DOWN ARROW
                case 2:
                    player_speed[0] = 1;
                    player_speed[1] = 0;
                    break;
                // 3 means LEFT ARROW
                case 3:
                    player_speed[0] = 0;
                    player_speed[1] = -1;
                    break;
                // 4 means RIGHT ARROW
                case 4:
                    player_speed[0] = 0;
                    player_speed[1] = 1;
                    break;
                // 5 means SPACEBAR
                case 5:
                    player_speed[0] = 0;
                    player_speed[1] = 0;
                    break;
            }

            // if 0 was read, quit
            if (!running) {
                break;
            }

        }

        // game logic
        player_pos[0] += player_speed[0];
        player_pos[1] += player_speed[1];

        // clear screen
        display_clear(drawer->display);

        // draw player
        display_set(drawer->display, (player_pos[0] % size[0] + size[0]) % size[0],
                    (player_pos[1] % size[1] + size[1]) % size[1], square);

        drawer_draw_display(drawer);
    }

    free(size);

    return 0;
}

int main(void) {
    Queue *queue = init_queue();
    Drawer *drawer = init_drawer();
    KeyListener *listener = init_keylistener(queue, drawer);

    keylistener_add_key(listener, CONST.K_ESC, 0);
    keylistener_add_key(listener, CONST.K_UARROW, 1);
    keylistener_add_key(listener, CONST.K_DARROW, 2);
    keylistener_add_key(listener, CONST.K_LARROW, 3);
    keylistener_add_key(listener, CONST.K_RARROW, 4);
    keylistener_add_key(listener, CONST.K_SPACEBAR, 5);
    drawer_set_fps(drawer, 30);

    drawer_start_thread(drawer, queue, gameloop);
    keylistener_handle_in(listener);

    return 0;
}