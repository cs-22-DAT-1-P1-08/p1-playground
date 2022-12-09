#include "main_view.h"
#include <curses.h>

typedef struct main_view_data_t {

} main_view_data_t;

typedef struct menu_item_t {
    char text[100];
    void (*callback)();
} menu_item_t;

int render_menu(WINDOW* window, menu_item_t items[], int item_count) {
    curs_set(0);

    int start_x = 0;
    int start_y = getcury(window) + (getcurx(window) != 0 ? 1 : 0);

    int current_item = 0;

    for (int i = 0; i < item_count; i++) {
        mvwprintw(window, start_y + i, 0, "[%c] %s\n", current_item == i ? 'X' : ' ', items[i].text);
    }
    wrefresh(window);
    keypad(window, true);

    while (true) {
        int inp = wgetch(window);
        if (inp == KEY_UP || inp == KEY_DOWN) {
            int prev_item = current_item;
            if (inp == KEY_UP) {
                if (current_item <= 0)
                    continue;
                current_item--;
            } else {
                if (current_item >= item_count - 1)
                    continue;
                current_item++;
            }

            int temp_x = getcurx(window);
            int temp_y = getcury(window);

            mvwprintw(window, start_y + prev_item, start_x + 1, " ");
            mvwprintw(window, start_y + current_item, start_x + 1, "X");

            wmove(window, temp_y, temp_x);
            wrefresh(window);
        }
        else if (inp == KEY_ENTER || inp == '\n' || inp == '\r' || inp == KEY_RIGHT) {
            curs_set(1);
            if (items[current_item].callback != NULL)
                items[current_item].callback();

            return current_item;
        }
    }

}

void on_shopping_list() {
    clear();
    printw("shopping list");
    refresh();
}

void on_quit() {
    clear();
    printw("quit pls");
    refresh();
}

WINDOW* create_main_view(WINDOW *parent) {
    WINDOW *window;
    window = subwin(parent, getmaxy(parent), getmaxx(parent), 0, 0);
}

void destroy_main_view(WINDOW* main_view) {
    wclear(main_view);
    wrefresh(main_view);
    delwin(main_view);
}

void render_main_view(WINDOW *window) {
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

    int selected_item = render_menu(window, menu_items, sizeof(menu_items) / sizeof(menu_items[0]));

    wclear(window);
    switch(selected_item) {
        case 0:
            wprintw(window, "Shopping list\n");
            break;
        case 1:
            wprintw(window, "Quit\n");
            break;
        default:
            break;
    }
    wrefresh(window);
    wgetch(window);

}