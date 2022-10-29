#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include "../header/display.h"

/* Initializes a new Display
 * The display size is initialized to the terminal's current size (in rows, columns)
 * The display array is allocated to the size rows*columns
 * TODO: Currently, resizing the terminal window while the engine is running is not supported
 * TODO: Allow up to 4 bytes to be available per character on screen (instead of just 1)
 * Return: Pointer to the initialized Display
 */
Display* init_display() {
    Display *new = malloc(sizeof(Display));
    display_update_size(new);
    new->_display_array = malloc((new->_rows*new->_columns)*sizeof(char));

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
            display_set(display, i, j, ' ');
        }
    }

}

/* Gets the character at the given position of a Display
 */
char display_get(Display *display, int row, int column) {
    return display->_display_array[row*display->_columns + column];
}

/* Sets the character at the given position of a Display to the given value
 */
void display_set(Display *display, int row, int column, char c) {
    display->_display_array[row*display->_columns + column] = c;
}
