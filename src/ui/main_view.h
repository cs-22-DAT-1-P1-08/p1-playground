#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include "curses.h"


enum MainViewMenuOptions {
    SHOPPING_LIST,
    QUIT
};

WINDOW* create_main_view(WINDOW *parent);
void destroy_main_view(WINDOW* main_view);
void render_main_view(WINDOW *window, enum MainViewMenuOptions *out_selected);

#endif