#ifndef SHOPPING_LIST_VIEW_H
#define SHOPPING_LIST_VIEW_H

#include <curses.h>
#include "../util/dlist.h"

void render_shopping_list(WINDOW *window, dlist_t* shopping_list);

#endif