#include <string.h>
#include "../../util/dlist.h"

typedef struct table_t {
    char* title;
    dlist_t* rows;
} table_t;

table_t* init_table() {

}

void free_table(table_t *table) {

}

void render_table(table_t *table) {
    dlist_node_t *row_node = NULL;
    dlist_node_t *column_node = NULL;
    dlist_t *temp_row = NULL;

    char *temp_field = NULL;
    int i;


    // Find longest row
    int table_width = 0;
    for (row_node = table->rows->head; row_node != NULL; row_node = row_node->next) {
        temp_row = row_node->data;

        if (temp_row->count > table_width)
            table_width = (int)temp_row->count;
    }

    // Loop: Columns
    // - Find longest word for each column
    int *column_widths = calloc(table_width, sizeof(int));
    for (row_node = table->rows->head; row_node != NULL; row_node = row_node->next) {
        temp_row = row_node->data;

        for (i = 0, column_node = temp_row->head; column_node != NULL; i++, column_node = column_node->next) {
            temp_field = column_node->data;
            if (strlen(temp_field) > column_widths[i])
                column_widths[i] = (int)strlen(temp_field);
        }
    }

    // Loop: Rows
    // - Find "biggest" field
    // - Print borders
    // - Print contents


    // Deallocate memory
    free(column_widths);
}