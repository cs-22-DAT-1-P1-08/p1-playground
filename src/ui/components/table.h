#ifndef TABLE_UI_COMPONENT_H
#define TABLE_UI_COMPONENT_H

#include <curses.h>
#include "../../util/dlist.h"

typedef struct table_t {
    char* title;
    dlist_t* rows;
    int padding_x;
    int padding_y;
    int* m_column_widths;
    int* m_row_heights;
} table_t;

table_t* init_table();
void free_table(table_t *table);
void table_add_row(table_t *table, int n, ...);

void render_table(WINDOW *window, table_t *table);

#endif