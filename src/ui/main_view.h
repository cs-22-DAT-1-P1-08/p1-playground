#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include "curses.h"

WINDOW* create_main_view(WINDOW *parent);
void destroy_main_view(WINDOW* main_view);
void render_main_view(WINDOW *window);

#endif