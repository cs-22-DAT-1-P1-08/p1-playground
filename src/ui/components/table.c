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

    // Don't worry about these :)
    dlist_node_t *row_node = NULL,
                 *column_node = NULL;
    table_row_t *temp_row = NULL;

    // Save cursor position
    int top = getcury(window),
        left = getcurx(window);

    // Get table dimensions
    int table_width = find_table_width(table);
    fill_row_column_dimensions(table, table_width);

    // Print borders
    wmove(window, top, left);

    int simple_row, prev_simple_row = 0, print_simple;
    for (i = 0, row_node = table->m_rows->head; i < table->m_rows->count; i++, row_node = row_node->next) {
        temp_row = row_node->data;
        simple_row = (temp_row->flags & ROW_SIMPLE) == ROW_SIMPLE;
        print_simple = simple_row && prev_simple_row;

        print_line_borders(window, table, table_width,print_simple ? ' ' : '-',print_simple ? '|' : '+');

        for (j = 0; j < table->m_row_heights[i] + table->padding_y * 2; j++)
            print_line_borders(window, table, table_width, ' ', '|');

        if (i == table->m_rows->count - 1)
            print_line_borders(window, table, table_width, '-', '+');

        prev_simple_row = simple_row;
    }

    int bottom = getcury(window);

    // Print fields
    char *line, *rest, *temp_field = NULL;

    int offset_x, offset_y = 0;
    int temp_x, temp_y;
    for (i = 0, row_node = table->m_rows->head; row_node != NULL; i++, row_node = row_node->next) {
        temp_row = row_node->data;

        int is_bold = (temp_row->flags & ROW_BOLD) == ROW_BOLD;
        int align_center = (temp_row->flags & ROW_ACENTER) == ROW_ACENTER;
        int align_right = (temp_row->flags & ROW_ARIGHT) == ROW_ARIGHT;

        if (is_bold) attron(attron(A_BOLD));

        offset_x = 0;
        offset_y += 1;

        for (j = 0, column_node = temp_row->fields->head; column_node != NULL; j++, column_node = column_node->next) {
            offset_x += 1;
            temp_x = left + offset_x + table->padding_x;
            temp_y = top + offset_y + table->padding_y;

            // Duplicate to avoid changing table data (strtok modifies string value)
            rest = temp_field = strdup(column_node->data);

            for (k = 0; (line = strtok_r(rest, "\n", &rest)); k++) {
                int align_offset = 0;
                if (align_center || align_right) {
                    align_offset = (int)(table->m_column_widths[j] - strlen(line));
                    if (align_center) align_offset /= 2;
                }

                mvwprintw(window, temp_y + k, temp_x + align_offset, "%s", line);
            }

            free(temp_field);
            offset_x += table->m_column_widths[j] + table->padding_x * 2;
        }

        if (is_bold) attroff(attron(A_BOLD));

        offset_y += table->m_row_heights[i] + table->padding_y * 2;
    }

    wmove(window, bottom, 0);

    // Deallocate memory
    free(table->m_column_widths);
    free(table->m_row_heights);
}

table_t* init_table() {
    table_t *table = calloc(1, sizeof(table_t));
    table->m_rows = calloc(1, sizeof(dlist_t));

    table->padding_x = 0;
    table->padding_y = 0;

    return table;
}

void table_add_row(table_t *table, int flags, int n, ...) {
    va_list ptr;
    va_start(ptr, n);

    table_row_t *row = calloc(1, sizeof(table_row_t));
    row->fields = calloc(1, sizeof(dlist_t));

    row->flags = flags;

    for (int i = 0; i < n; i++)
        dlist_add(row->fields, va_arg(ptr, char*));

    dlist_add(table->m_rows, row);

    va_end(ptr);
}

void table_add_row_array(table_t *table, int flags, int n, char* arr[]) {
    table_row_t *row = calloc(1, sizeof(table_row_t));
    row->fields = calloc(1, sizeof(dlist_t));

    row->flags = flags;

    for (int i = 0; i < n; i++)
        dlist_add(row->fields, arr[i]);

    dlist_add(table->m_rows, row);
}

table_row_t* table_add_empty_row(table_t *table, int flags, int n) {
    table_row_t *row = calloc(1, sizeof(table_row_t));
    row->fields = calloc(1, sizeof(dlist_t));

    row->flags = flags;

    for (int i = 0; i < n; i++)
        dlist_add(row->fields, "");

    dlist_add(table->m_rows, row);
    return row;
}

void table_row_update_field(table_row_t *row, int field_index, char* new_value) {
    dlist_get_at(row->fields, field_index)->data = new_value;
}

void free_table(table_t *table) {
    if (table == NULL)
        return;

    if (table->title != NULL)
        free(table->title);

    // TODO: Determine how to free fields only if heap allocated
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
    for (dlist_node_t *row_node = table->m_rows->head; row_node != NULL; row_node = row_node->next) {
        table_row_t *temp_row = row_node->data;

        if (temp_row->fields->count > table_width)
            table_width = (int)temp_row->fields->count;
    }

    return table_width;
}

void fill_row_column_dimensions(table_t *table, int table_width) {
    table->m_column_widths = calloc(table_width, sizeof(int));
    table->m_row_heights = calloc(table->m_rows->count, sizeof(int));

    /* These local variables are initialized within the loops */
    char *temp_field;
    dlist_node_t *row_node, *column_node;
    int i, j;

    for (i = 0, row_node = table->m_rows->head; row_node != NULL; i++, row_node = row_node->next) {
        table_row_t *temp_row = row_node->data;

        for (j = 0, column_node = temp_row->fields->head; column_node != NULL; j++, column_node = column_node->next) {
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
