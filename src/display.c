#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../header/display.h"

/* Initializes a new Display
 * The display size is initialized to the terminal's current size (in rows, columns)
 * The display array is allocated to the size 4*rows*columns, since it is a table of size rows*columns with CELLBYTES bytes per cell
 * TODO: Currently, resizing the terminal window while the engine is running is not supported
 * Return: Pointer to the initialized Display
 */
Display* init_display() {
    Display *new = malloc(sizeof(Display));
    display_update_size(new);
    // display array is table of size
    new->_display_array = malloc(new->_rows*new->_columns*sizeof(char)*CELLBYTES);

    new->empty[0] = ' ';
    for (int i = 1; i < CELLBYTES; ++i) {
        new->empty[i] = '\0';
    }

    return new;
}

/* Deletes a Display
 * This removes the Display from memory, as well as its internal display array
 */
void delete_display(Display *display) {
    free(display->_display_array);
    free(display);
}

/* Updates the Display size by getting the current terminal size
 * This currently doesn't affect the display array, meaning this function should only be used by init_display
 */
void display_update_size(Display *display) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    display->_rows = w.ws_row;
    display->_columns = w.ws_col;
}

/* Gets the current display size of a Display
 * Returns: int[2] containing rows ([0]) and columns ([1])
 * The returned value is dynamically allocated and should be freed when it is no longer needed
 */
int* display_get_size(Display *display) {
    display_update_size(display);
    int *size = malloc(2*sizeof(int));
    size[0] = display->_rows;
    size[1] = display->_columns;

    return size;
}

/* Clears a Display
 * This corresponds to replacing every existing character in the display with a whitespace character
 */
void display_clear(Display *display) {
    display_update_size(display);
    for (int i = 0; i < display->_rows; ++i) {
        for (int j = 0; j < display->_columns; ++j) {
            display_set_exact(display, i, j, display->empty);
        }
    }
}

/* Gets the character at the given position of a Display
 * The returned value should be freed after use
 * Return: char* of size CELLBYTES containing character(s) at cell
 */
char* display_get(Display *display, int row, int column) {
    char *res = malloc(CELLBYTES*sizeof(char));
    memcpy(res, &display->_display_array[CELLBYTES*(row*display->_columns + column)], CELLBYTES);
    return res;
}

/* Sets the character at the given position of a Display to the given value
 * const char *c: Should be a char array of size CELLBYTES or less; if it is longer, only up to CELLBYTES are used
 *  only single characters should be contained in the array (including those that require more than 1 byte for representation)
 */
void display_set(Display *display, int row, int column, const char *c) {
    int end_index = -1;
    for (int i = 0; i < CELLBYTES; ++i) {
        if (c[i] == '\0') {
            end_index = i;
            break;
        }
    }

    if (end_index == -1 || end_index == CELLBYTES - 1) {
        memcpy(&display->_display_array[CELLBYTES*(row*display->_columns+column)], c, CELLBYTES*sizeof(char));
        return;
    }

    int index = CELLBYTES * (row * display->_columns + column);
    memcpy(&display->_display_array[index], c, end_index*sizeof(char));
    memset(&display->_display_array[index + end_index], '\0', (CELLBYTES - end_index)*sizeof(char));
}

/* Similar to display_set, but assumes const char *c is exactly CELLBYTES in length, padded with the appropriate amount
 *  of '\0' characters at the end if necessary, thus avoiding unnecessary checks
 * To be used by display_clear
 */
void _display_set_exact(Display *display, int row, int column, const char *c) {
    memcpy(&display->_display_array[CELLBYTES*(row*display->_columns+column)], c, CELLBYTES*sizeof(char));
}
