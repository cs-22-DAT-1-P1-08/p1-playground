#include "table.h"
#include <string.h>
#include <curses.h>

// Private function prototypes
void print_line_borders(WINDOW *window, table_t *table, int table_width, char fill, char border);
int find_table_width(table_t *table);
void fill_row_column_dimensions(table_t *table, int table_width);

void render_table(WINDOW *window, table_t *table) {
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

        for (j = 0; j < table->m_row_heights[i] + table->padding_y * 2; j++)
            print_line_borders(window, table, table_width, ' ', '|');

        if (i == table->rows->count - 1)
            print_line_borders(window, table, table_width, '-', '+');
    }

    int bottom = getcury(window);


    // Print fields
    dlist_node_t *row_node = NULL,
                 *column_node = NULL;
    dlist_t *temp_row = NULL;
    char *line, *rest, *temp_field = NULL;

    int offset_x, offset_y = 0;
    int temp_x, temp_y;
    for (i = 0, row_node = table->rows->head; row_node != NULL; i++, row_node = row_node->next) {
        temp_row = row_node->data;

        offset_x = 0;
        offset_y += 1;

        for (j = 0, column_node = temp_row->head; column_node != NULL; j++, column_node = column_node->next) {
            offset_x += 1;
            temp_x = left + offset_x + table->padding_x;
            temp_y = top + offset_y + table->padding_y;

            // Duplicate to avoid changing table data (strtok modifies string value)
            rest = temp_field = strdup(column_node->data);

            for (k = 0; (line = strtok_r(rest, "\n", &rest)); k++)
                mvwprintw(window, temp_y + k, temp_x, "%s", line);

            free(temp_field);
            offset_x += table->m_column_widths[j] + table->padding_x * 2;
        }

        offset_y += table->m_row_heights[i] + table->padding_y * 2;
    }

    wmove(window, bottom, 0);

    // Deallocate memory
    free(table->m_column_widths);
    free(table->m_row_heights);
}

table_t* init_table() {
    table_t *table = calloc(1, sizeof(table_t));
    table->rows = calloc(1, sizeof(dlist_t));

    table->padding_x = 0;
    table->padding_y = 0;

    return table;
}

void table_add_row(table_t *table, int n, ...) {
    va_list ptr;
    va_start(ptr, n);

    dlist_t *fields = calloc(1, sizeof(dlist_t));

    for (int i = 0; i < n; i++)
        dlist_add(fields, va_arg(ptr, char*));

    dlist_add(table->rows, fields);

    va_end(ptr);
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

void print_line_borders(WINDOW *window, table_t *table, int table_width, char fill, char border) {
    for (int j = 0; j < table_width; j++) {
        waddch(window, border);
        for (int k = 0; k < table->m_column_widths[j] + table->padding_x * 2; k++)
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
    int i, j;

    for (i = 0, row_node = table->rows->head; row_node != NULL; i++, row_node = row_node->next) {
        dlist_t *temp_row = row_node->data;

        for (j = 0, column_node = temp_row->head; column_node != NULL; j++, column_node = column_node->next) {
            temp_field = column_node->data;

            temp_field = strdup(column_node->data);
            char *line, *rest = temp_field;

            int field_height = 0;
            while ((line = strtok_r(rest, "\n", &rest)) != NULL) {
                if (strlen(line) > table->m_column_widths[j])
                    table->m_column_widths[j] = (int)strlen(line);

                field_height++;
            }

            if (field_height > table->m_row_heights[i])
                table->m_row_heights[i] = field_height;

            free(temp_field);
        }
    }
}
