#include <curses.h>
#include <string.h>

struct demo_products
{
    char* product_name;
    char* amount;
    int price;
};

int check_longest_word(char* test, char* test2)
{
    int result;
    int maxLength = 0;

    if (strlen(test) > strlen(test2))
        maxLength = strlen(test);
    else if (strlen(test) < strlen(test2))
        maxLength = strlen(test2);

    result = maxLength;

    return result;
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

void print_shopping_list(char* test, struct demo_products products2[])
{


    //int size = sizeof(products2->product_name) / sizeof(products2[0].product_name);

    int number;

    mvprintw(0, 30, "Grocery list");

    lines(test,1);

    mvprintw(1, 12+strlen(test), "+--- Butik1 (0) ---+---- Butik2 (1) ----+\n");

    int i = 0;
    while (i < 2+1)
    {
        mvprintw(2+i, 10, "|%s | 12.25 kr (1L)    | 9.00 kr (1L)       |", products2[0+i].product_name);

        i++;
    }

    lines(test,3+i);
    mvprintw(3+i, 12+strlen(test), "+------------------+--------------------+\n");

    mvprintw(4+i, 10, "|Total");
    number = strlen("|Distance") + space(test, "Distance") + 1;
    mvprintw(4+i, number, "| 18.25 kr         | 16.50 kr           |\n");

    lines(test,5+i);
    mvprintw(5+i, 12+strlen(test), "+------------------+--------------------+\n\n");

    mvprintw(6+i, 10, "|Distance");
    number = strlen("|Distance") + space(test, "Distance") + 1;
    mvprintw(6+i, number, "| 551 meters away  | 2224 meters away   |\n");

    lines(test,7+i);
    mvprintw(7+i, 12+strlen(test), "+------------------+--------------------+\n\n");
}

int main()
{
    struct demo_products products1[40];

    int i = 0;

    //int size = sizeof(products1) / sizeof(products1[0]); //Size of cards struct array

    char name_test1[20] = "Product 1";
    char name_test2[20] = "Product 2";
    char input[30];
    int price_test1 = 30;
    int price_test2 = 12;

    initscr(); // Start curses mode

    mvprintw(1, 1, "Enter products name (type q to stop)\n");

    while (strcmp(input, "q") != 0)
    {
        getstr(input);

        if (strcmp(input, "q") == 0) break;
        else products1[i].product_name = input;

        i++;
    }

    for (int j = 0; j < 3; ++j)
    {
        mvprintw(2+j, 100, "%s", products1[j].product_name);
    }

    print_shopping_list(name_test1, products1);

    refresh(); // Update screen
    getchar();
    endwin(); // End curses mode
    return 0;
}