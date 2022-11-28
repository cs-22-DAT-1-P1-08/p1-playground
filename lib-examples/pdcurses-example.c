#include <curses.h>
#include <string.h>

unsigned int gcd(unsigned int large, unsigned int small)
{
    unsigned int remainder;
    while (small > 0){
        remainder = large % small;
        large = small;
        small = remainder;
    }
    return large;
}

int space(char* test, char* test2)
{
    int number;
    for (int i = 0; i < strlen(test)-strlen(test2)+1; ++i) {
        number++;
    }
    return number;
}

void lines(char* test, int row)
{
    mvprintw(row, 10, "+--");
    for (int i = 0; i < strlen(test); ++i) {
        mvprintw(row, 12+i,"-");
    }
}

void print_shopping_list(char* test, char* test2)
{
    int number;

    lines(test,1);

    mvprintw(1, 12+strlen(test), "+--- Butik1 (0) ---+---- Butik2 (1) ----+\n");
    mvprintw(2, 10, "|%s | 12.25 kr (1L)    | 9.00 kr (1L)       |\n", test);
    mvprintw(3, 10, "|%s | 6.00 kr (500G)   | 7.50 kr (500G)     |\n", test2);

    lines(test,4);
    mvprintw(4, 12+strlen(test), "+------------------+--------------------+\n");

    mvprintw(5, 10, "|Total");
    number = strlen("|Distance") + space(test, "Distance") + 1;
    mvprintw(5, number, "| 46.25 kr         | 57.95 kr           |\n");

    lines(test,6);
    mvprintw(6, 12+strlen(test), "+------------------+--------------------+\n\n");

    mvprintw(7, 10, "|Distance");
    number = strlen("|Distance") + space(test, "Distance") + 1;
    mvprintw(7, number, "| 551 meters away  | 2224 meters away   |\n");

    lines(test,8);
    mvprintw(8, 12+strlen(test), "+------------------+--------------------+\n\n");
}

int main()
{
    char name_test1[20] = "Product 1";
    char name_test2[20] = "Product 2";
    int price_test1 = 30;
    int price_test2 = 12;

    initscr(); // Start curses mode

    print_shopping_list(name_test1, name_test2);

    refresh(); // Update screen
    getchar();
    endwin(); // End curses mode
    return 0;
}