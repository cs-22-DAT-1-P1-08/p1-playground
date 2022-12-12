#include "main_view.h"
#include "components/arrow_menu.h"
#include <curses.h>

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

void render_main_view(WINDOW *window, enum MainViewMenuOptions *out_selected) {
    wprintw(window, "__________                           .__         ___________.__            .___            \n"
           "\\______   \\_____ _______  _________  |__| ____   \\_   _____/|__| ____    __| _/___________ \n"
           " |    |  _/\\__  \\\\_  __ \\/ ___\\__  \\ |  |/    \\   |    __)  |  |/    \\  / __ |/ __ \\_  __ \\\n"
           " |    |   \\ / __ \\|  | \\/ /_/  > __ \\|  |   |  \\  |     \\   |  |   |  \\/ /_/ \\  ___/|  | \\/\n"
           " |______  /(____  /__|  \\___  (____  /__|___|  /  \\___  /   |__|___|  /\\____ |\\___  >__|   \n"
           "        \\/      \\/     /_____/     \\/        \\/       \\/            \\/      \\/    \\/");
    wprintw(window, "\n\n\n");

    menu_item_t menu_items[] = {
            { "Create shopping list" },
            { "Quit" }
    };

    *out_selected = render_menu(window, menu_items, sizeof(menu_items) / sizeof(menu_items[0]));
}