#include <stdio.h>
#include <curses.h>

int main() {
    initscr(); // Start curses mode
    noecho();
    cbreak();

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW * inputwin = newwin(6, xMax-12, yMax-8, 5);
    box(inputwin,0,0);
    refresh();
    wrefresh(inputwin);
    keypad(inputwin, true);

    //mvprintw(10, 10, "\U0001F600");
    getchar();
    endwin(); // End curses mode
    return 0;
}