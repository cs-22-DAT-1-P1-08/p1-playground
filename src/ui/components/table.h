#ifndef TABLE_UI_COMPONENT_H
#define TABLE_UI_COMPONENT_H

#include <curses.h>
#include "../../util/dlist.h"

#define ROW_DEFAULT 0
#define ROW_SIMPLE 1 << 0
#define ROW_BOLD 1 << 1
#define ROW_ACENTER 1 << 2
#define ROW_ARIGHT 1 << 3

typedef struct table_t {
    char* title;
    int padding_x;
    int padding_y;
    dlist_t* m_rows;
    int* m_column_widths;
    int* m_row_heights;
} table_t;

typedef struct table_row_t {
    int flags;
    dlist_t *fields;
} table_row_t;

table_t* init_table();
void free_table(table_t *table);

void table_add_row(table_t *table, int flags, int n, ...);
void table_add_row_array(table_t *table, int flags, int n, char* arr[]);
table_row_t* table_add_empty_row(table_t *table, int flags, int n);
void table_row_update_field(table_row_t *row, int field_index, char* new_value);

void render_table(WINDOW *window, table_t *table);

#endif