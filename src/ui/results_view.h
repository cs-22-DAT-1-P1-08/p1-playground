#ifndef RESULTS_VIEW_H
#define RESULTS_VIEW_H

#include <curses.h>
#include "../util/dlist.h"
#include "../store.h"

enum ResultViewMenuOptions {
    RVM_SHOPPING_LIST,
    RVM_MAIN_SCREEN
};

typedef struct results_view_data_t {
    dlist_t *shopping_list;
    store_t **stores;
    size_t store_count;
    location_t *home;
    char* here_api_key;
} results_view_data_t;

int render_results_view(WINDOW *window, results_view_data_t *data);

#endif