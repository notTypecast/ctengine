#include "../header/ctengine.h"
#include <time.h>
#include <stdlib.h>
#define ENEMIES 5

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

    // initialize enemies
    int enemy_positions[ENEMIES][2];
    int enemy_speeds[ENEMIES][2];

    // randomly decide initial enemy values
    for (int i = 0; i < ENEMIES; ++i) {
        enemy_positions[i][0] = rand() % size[0];
        enemy_positions[i][1] = rand() % size[1];

        enemy_speeds[i][0] = rand() % 2;
        enemy_speeds[i][1] = rand() % 2;
    }

    // UTF-8 character representing enemy
    const char *enemy = "☀";

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

        display_clear(drawer->display);

        // game logic
        player_pos[0] += player_speed[0];
        player_pos[1] += player_speed[1];

        // draw player
        display_set(drawer->display, (player_pos[0] % size[0] + size[0]) % size[0],
                    (player_pos[1] % size[1] + size[1]) % size[1], square);

        // draw enemies
        for (int i = 0; i < ENEMIES; ++i) {
            if (player_pos[0] == enemy_positions[i][0] && player_pos[1] == enemy_positions[i][1]) {
                queue->finished = 1;
                drawer_set_exit_msg(drawer, "You lose!");
                return 0;
            }

            enemy_positions[i][0] += enemy_speeds[i][0];
            enemy_positions[i][1] += enemy_speeds[i][1];

            display_set(drawer->display, (enemy_positions[i][0] % size[0] + size[0]) % size[0],
                        (enemy_positions[i][1] % size[1] + size[1]) % size[1], enemy);
        }

        drawer_draw_display(drawer);
    }

    free(size);

    return 0;
}

int main(void) {
    srand(time(NULL));

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