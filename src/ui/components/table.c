#include <string.h>
#include <curses.h>
#include "../../util/dlist.h"

typedef struct table_t {
    char* title;
    dlist_t* rows;
    int* m_column_widths;
    int* m_row_heights;
} table_t;

table_t* init_table() {
    table_t *table = calloc(1, sizeof(table_t));
    table->rows = calloc(1, sizeof(dlist_t));

    return table;
}

void free_table(table_t *table) {
    if (table == NULL)
        return;

    if (table->title != NULL)
        free(table->title);

    // TODO: Determine how to free both rows, columns and optionally fields
    //if (table->rows != NULL)
    //    dlist_free_all(table->rows, )

    free(table);
}

// Private function prototypes
void print_line_borders(WINDOW *window, table_t *table, int table_width, char fill, char border);
int find_table_width(table_t *table);
void fill_row_column_dimensions(table_t *table, int table_width);

void render_table(WINDOW *window, table_t *table) {
    dlist_node_t *row_node = NULL;
    dlist_node_t *column_node = NULL;
    dlist_t *temp_row = NULL;
    char *temp_field = NULL;

    // Loop control variables, initialized when used.
    int i, j, k;

    // Save cursor position
    int top = getcury(window),
        left = getcurx(window);

    // Get table dimensions
    int table_width = find_table_width(table);
    fill_row_column_dimensions(table, table_width);

    // Print borders
    wmove(window, top, left);
    for (i = 0; i < table->rows->count; i++) {
        print_line_borders(window, table, table_width, '-', '+');

        for (j = 0; j < table->m_row_heights[i]; j++)
            print_line_borders(window, table, table_width, ' ', '|');

        if (i == table->rows->count - 1)
            print_line_borders(window, table, table_width, '-', '+');
    }


    // Print fields
    wmove(window, top, left);
    int offset_y = 0;
    for (i = 0, row_node = table->rows->head; row_node != NULL; i++, row_node = row_node->next) {
        temp_row = row_node->data;

        int offset_x = 0;
        offset_y += 1;

        for (j = 0, column_node = temp_row->head; column_node != NULL; j++, column_node = column_node->next) {
            offset_x += 1;

            // Duplicate to avoid changing table data (strtok modifies string value)
            temp_field = strdup(column_node->data);
            char *line, *rest = temp_field;

            k = 0;
            while ((line = strtok_r(rest, "\n", &rest)) != NULL)
                mvwprintw(window, top + offset_y + k++, left + offset_x, "%s", temp_field);


            free(temp_field);
            offset_x += table->m_column_widths[j];
        }

        offset_y += table->m_row_heights[i];
    }

    // Deallocate memory
    free(table->m_column_widths);
    free(table->m_row_heights);
}


void print_line_borders(WINDOW *window, table_t *table, int table_width, char fill, char border) {
    for (int j = 0; j < table_width; j++) {
        waddch(window, border);
        for (int k = 0; k < table->m_column_widths[j]; k++)
            waddch(window, fill);
    }
    waddch(window, border);
    waddch(window, '\n');
}

int find_table_width(table_t *table) {
    int table_width = 0;
    for (dlist_node_t *row_node = table->rows->head; row_node != NULL; row_node = row_node->next) {
        dlist_t *temp_row = row_node->data;

        if (temp_row->count > table_width)
            table_width = (int)temp_row->count;
    }

    return table_width;
}

void fill_row_column_dimensions(table_t *table, int table_width) {
    table->m_column_widths = calloc(table_width, sizeof(int));
    table->m_row_heights = calloc(table->rows->count, sizeof(int));

    /* These local variables are initialized within the loops */
    char *temp_field;
    dlist_node_t *row_node, *column_node;
    int i, j, k;

    for (i = 0, row_node = table->rows->head; row_node != NULL; i++, row_node = row_node->next) {
        dlist_t *temp_row = row_node->data;

        for (j = 0, column_node = temp_row->head; column_node != NULL; j++, column_node = column_node->next) {
            temp_field = column_node->data;
            if (strlen(temp_field) > table->m_column_widths[j])
                table->m_column_widths[j] = (int)strlen(temp_field);

            int field_height;
            for (k = 0, field_height = 1; temp_field[k]; k++)
                field_height += (temp_field[k] == '\n');

            if (field_height > table->m_row_heights[i])
                table->m_row_heights[i] = field_height;
        }
    }
}
