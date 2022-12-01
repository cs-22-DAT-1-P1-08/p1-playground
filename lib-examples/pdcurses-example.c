#include <curses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
    char product_name[30];
    char store[20];
    char amount[10];
    double price;
    int distance;
} demo_products;

int rnd_nr(int size)
{
    return rand() % size;
}

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

void print_shopping_list(char* test, demo_products products2[], int size)
{
    int number;
    double total_price1;
    double total_price2;

    mvprintw(0, 30, "Grocery list");

    lines(test,1);

    mvprintw(1, 12+strlen(test), "+--- %s (0) ---+---- %s (1) ----+\n", products2[rnd_nr(size)].store, products2[rnd_nr(size)].store);

    int i = 0;
    while (i < size-1) //Minus 1 because we don't want "q" to be there
    {
        mvprintw(2+i, 10, "|%s | %.2lf kr (%s)    | %.2lf kr (%s)       |", products2[i].product_name, products2[i].price, products2[i].amount, products2[i+1].price, products2[i].amount);

        total_price1 = total_price1 + products2[i].price;
        total_price2 = total_price2 + products2[i+1].price;
        i++;
    }

    lines(test,2+i);
    mvprintw(2+i, 12+strlen(test), "+------------------+--------------------+\n");

    mvprintw(3+i, 10, "|Total");
    number = strlen("|Distance") + space(test, "Distance") + 1;
    mvprintw(3+i, number, "| %.2lf kr         | %.2lf kr           |\n", total_price1, total_price2);

    lines(test,4+i);
    mvprintw(4+i, 12+strlen(test), "+------------------+--------------------+\n\n");

    mvprintw(5+i, 10, "|Distance");
    number = strlen("|Distance") + space(test, "Distance") + 1;
    mvprintw(5+i, number, "| %d meters away  | %d meters away   |\n", products2[rnd_nr(size)].distance, products2[rnd_nr(size)].distance);

    lines(test,6+i);
    mvprintw(6+i, 12+strlen(test), "+------------------+--------------------+\n\n");
}

int main()
{
    //int size = sizeof(products1) / sizeof(products1[0]);

    srand(time(NULL)); // This is just for testing and won't be in the final

    char name_test1[20] = "Product 1";
    char name_test2[20] = "Product 2";
    char input[30];
    int price_test1 = 30;
    int price_test2 = 12;
    int i = 0;
    int size = 0;

    demo_products products1[40];

    char* demo_store[] = {"Netto", "Fakta", "Aldi", "Lidl", "Rema 1000", "Foetex", "Irma", "SuperBrugsen", "Kvickly", "Spar"};
    char* demo_weight[] = {"1L", "250g", "500g", "1kg", "5dl", "2kg", "2L", "7dl", "25g", "800g"};
    double demo_price[] = {30.95, 40.00, 12.99, 19.00, 22.50, 49.50, 120.00, 10.00, 7.49, 150};
    int demo_distance[] = {551, 181, 2224, 752, 42, 576, 1296, 392, 226, 127};

    initscr(); // Start curses mode

    mvprintw(1, 1, "Enter products name (type q to stop)\n");

    while (strcmp(input, "q") != 0) //This is purely for testing data and won't be in the final version
    {
        getstr(input);
        strcpy(products1[i].product_name,input);
        strcpy(products1[i].store,demo_store[i]);
        strcpy(products1[i].amount,demo_weight[i]);
        products1[i].price = demo_price[i];
        products1[i].distance = demo_distance[i];

        i++;
        size++;
    }

    clear();

    print_shopping_list(name_test1, products1, size);

    refresh(); // Update screen
    getchar();
    endwin(); // End curses mode
    return 0;
}