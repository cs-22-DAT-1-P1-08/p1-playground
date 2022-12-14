#include "shopping_list_view.h"
#include "components/arrow_menu.h"
#include "../util/dlist.h"
#include <stdlib.h>
#include <string.h>

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

        switch (menu_result) {
            case 0: // Add item
                wprintw(window, "\n\n");
                wprintw(window, "Enter new item: ");

                // Temporary terminal mode
                curs_set(1);
                echo();

                // Get input
                char input[50];
                getstr(input);

                // Convert to char pointer
                char *new_item = calloc(strlen(input) + 1, sizeof(char));
                strcpy(new_item, input);

                // Reset terminal mode
                curs_set(0);
                noecho();

                dlist_add(shopping_list, new_item);
                wrefresh(window);
                break;
            case 1: // Edit item
                wprintw(window, "\n\n");
                wprintw(window, "Enter item number: ");

                // Temporary terminal mode
                curs_set(1);
                echo();

                int item_number;
                scanw(" %d", &item_number);
                while (item_number < 1 || item_number >= shopping_list->count + 1) {
                    wprintw(window, "You stupid! Try again: ");
                    scanw(" %d", &item_number);
                }

                dlist_node_t *line_node = dlist_get_at(shopping_list, item_number - 1);
                char* line = line_node->data;
                wprintw(window, "Editing item: %s\nNew item text: ", line);

                // Get input
                char edit_input[50];
                getstr(edit_input);

                // Convert to char pointer
                line_node->data = calloc(strlen(edit_input) + 1, sizeof(char));
                strcpy(line_node->data, edit_input);

                // Reset terminal mode
                curs_set(0);
                noecho();

                wrefresh(window);
                break;
            case 2: // remove item
                wprintw(window, "\n\n");
                wprintw(window, "Enter item number: ");

                // Temporary terminal mode
                curs_set(1);
                echo();

                int remove_item_number;
                scanw(" %d", &remove_item_number);
                while (remove_item_number < 1 || remove_item_number >= shopping_list->count + 1) {
                    wprintw(window, "You stupid! Try again: ");
                    scanw(" %d", &remove_item_number);
                }

                dlist_remove_at(shopping_list, remove_item_number - 1);

                // Reset terminal mode
                curs_set(0);
                noecho();

                wrefresh(window);
                break;
            case 3:
                is_done = true;
                break;
        }

        wrefresh(window);
    }

    wclear(window);
    wrefresh(window);
}