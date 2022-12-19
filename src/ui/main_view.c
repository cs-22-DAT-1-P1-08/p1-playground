#include "main_view.h"
#include "components/arrow_menu.h"
#include "shopping_list_view.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>

WINDOW* create_main_view(WINDOW *parent) {
    WINDOW *window;
    window = subwin(parent, getmaxy(parent), getmaxx(parent), 0, 0);
    return window;
}

void destroy_main_view(WINDOW* main_view) {
    wclear(main_view);
    wrefresh(main_view);
    delwin(main_view);
}

void render_main_view(WINDOW *window, enum MainViewMenuOptions *out_selected, location_t *address) {
    int done = 0;
    while (done == 0) {
        wclear(window);
        wprintw(window, "__________                           .__         ___________.__            .___            \n"
                        "\\______   \\_____ _______  _________  |__| ____   \\_   _____/|__| ____    __| _/___________ \n"
                        " |    |  _/\\__  \\\\_  __ \\/ ___\\__  \\ |  |/    \\   |    __)  |  |/    \\  / __ |/ __ \\_  __ \\\n"
                        " |    |   \\ / __ \\|  | \\/ /_/  > __ \\|  |   |  \\  |     \\   |  |   |  \\/ /_/ \\  ___/|  | \\/\n"
                        " |______  /(____  /__|  \\___  (____  /__|___|  /  \\___  /   |__|___|  /\\____ |\\___  >__|   \n"
                        "        \\/      \\/     /_____/     \\/        \\/       \\/            \\/      \\/    \\/");
        wprintw(window, "\n\n\n");
        wprintw(window, "Street name: %s\nHouse number: %s\nPostal code: %s\n\n",
                (address) ? address->street : "N/A",
                (address) ? address->houseNumber : "N/A",
                (address) ? address->postalCode : "N/A"
        );

        menu_item_t menu_items[] = {
                {"Create shopping list"},
                {"Edit address"},
                {"Quit"}
        };

        *out_selected = render_menu(window, menu_items, sizeof(menu_items) / sizeof(menu_items[0]));

        if (*out_selected == EDIT_ADDRESS) {
            wprintw(window, "\n\n");
            wprintw(window, "Enter street name: ");
            strcpy(address->street, text_input_field(window));
            wprintw(window, "Enter house number: ");
            strcpy(address->houseNumber, text_input_field(window));
            wprintw(window, "Enter postal code: ");
            strcpy(address->postalCode, text_input_field(window));
        } else done = 1;
    }
}