#ifndef TENGINE_DISPLAY_H
#define TENGINE_DISPLAY_H
// constant defining bytes per screen cell (def. 4)
#define CELLBYTES 4

/* Defines a Display, which is used to represent the terminal screen
 * int _rows: number of rows of the current display
 * int _columns: number of columns of the current display
 * char *_display_array: 2D array of "cells", each of which represents one character on the terminal screen
 * char *empty: Whitespace char followed by CELLBYTES*'\0'
 */
typedef struct {
    int _rows;
    int _columns;
    char *_display_array;
    char empty[CELLBYTES];
} Display;

// Display operations
Display* init_display();
void delete_display(Display *display);
void display_update_size(Display *display);
int* display_get_size(Display *display);
void display_clear(Display *display);
char* display_get(Display *display, int row, int column);
void display_set(Display *display, int row, int column, const char *c);
void display_set_exact(Display *display, int row, int column, const char *c);

#endif //TENGINE_DISPLAY_H
