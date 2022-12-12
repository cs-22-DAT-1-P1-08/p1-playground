#include "arrow_menu.h"
#include <curses.h>

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