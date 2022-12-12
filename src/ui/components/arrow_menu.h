#ifndef ARROW_MENU_H
#define ARROW_MENU_H

#include <curses.h>

typedef struct menu_item_t {
    char text[100];
    void (*callback)();
} menu_item_t;

int render_menu(WINDOW* window, menu_item_t items[], int item_count);

#endif