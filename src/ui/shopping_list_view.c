#include "shopping_list_view.h"
#include "components/arrow_menu.h"
#include "../util/dlist.h"
#include <stdlib.h>
#include <string.h>

char* text_input_field(WINDOW *window);
int integer_input_field(WINDOW *window, int min, int max);

void render_shopping_list(WINDOW *window, dlist_t* shopping_list) {
    menu_item_t menu[] = {
            { "Add item" },
            { "Edit item" },
            { "Remove item" },
            { "Done" }
    };

    bool is_done = false;

    while (is_done == false) {
        wclear(window);
        wprintw(window, "-------------------------------------------------\n");
        wprintw(window, "-                  Shopping list                -\n");
        wprintw(window, "-------------------------------------------------\n");
        wprintw(window, "Please fill in a list of items. The amount can\n");
        wprintw(window, "be specified as part of the text, e.g. 500 gram.\n");
        wprintw(window, "-------------------------------------------------\n");

        for (int i = 0; i < shopping_list->count; i++) {
            attron(COLOR_PAIR(1));
            wprintw(window, "%d ", i + 1);
            attroff(COLOR_PAIR(1));
            wprintw(window, "%s\n", (char*)dlist_get_at(shopping_list, i)->data);
        }

        if (shopping_list->count > 0)
            wprintw(window, "-------------------------------------------------\n");
        int menu_result = render_menu(window, menu, sizeof(menu) / sizeof(menu[0]));

        int item_number;
        switch (menu_result) {
            case 0: // Add item
                wprintw(window, "\n\n");
                wprintw(window, "Enter new item: ");

                dlist_add(shopping_list, text_input_field(window));
                break;
            case 1: // Edit item
                wprintw(window, "\n\n");
                wprintw(window, "Enter item number: ");

                // Get item
                item_number = integer_input_field(window, 1, (int)shopping_list->count);
                dlist_node_t *line_node = dlist_get_at(shopping_list, item_number - 1);
                char* line = line_node->data;

                // Get input
                wprintw(window, "Editing item: %s\nNew item text: ", line);

                free(line_node->data);
                line_node->data = text_input_field(window);
                break;
            case 2: // Remove item
                wprintw(window, "\n\n");
                wprintw(window, "Enter item number: ");

                item_number = integer_input_field(window, 1, (int)shopping_list->count);
                free(dlist_remove_at(shopping_list, item_number - 1));
                break;
            default:
            case 3:
                is_done = true;
                break;
        }

        wrefresh(window);
    }

    wclear(window);
    wrefresh(window);
}

char* text_input_field(WINDOW *window) {
    // Temporary terminal mode
    int prev_cursor_state = curs_set(1);
    echo();

    /* Get raw input */
    char input[50];
    wgetstr(window, input);

    /* Allocate to heap memory */
    char *result = calloc(strlen(input) + 1, sizeof(char));
    strcpy(result, input);

    /* Reset terminal mode */
    curs_set(prev_cursor_state);

    // "echo state" does not have a getter, we assume that previous state was noecho
    noecho();

    return result;
}

int integer_input_field(WINDOW *window, int min, int max) {
    // Temporary terminal mode
    int prev_cursor_state = curs_set(1);
    echo();

    int item_number;
    scanw(" %d", &item_number);
    while (item_number < min || item_number > max) {
        wprintw(window, "Invalid input! Try again: ");
        scanw(" %d", &item_number);
    }

    /* Reset terminal mode */
    curs_set(prev_cursor_state);

    // "echo state" does not have a getter, we assume that previous state was noecho
    noecho();
    return item_number;
}