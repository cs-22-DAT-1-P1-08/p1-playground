#include "results_view.h"
#include <curses.h>
#include <stdlib.h>

//a struct for fake data
typedef struct {
    char product_name[30];
    char store[20];
    char amount[10];
    double price;
    int distance;
} demo_products;

//Checks for the longest product name and changes the size of the box
int check_longest_word(demo_products products2[], int size)
{
    int result;
    int maxLength = 9;
    char* word;

    for (int i = 0; i < size; ++i)
    {
        word = products2[i].product_name;
        if (strlen(word) > maxLength)
            maxLength = strlen(word);

        result = maxLength;
    }

    return result;
}

//Changes the size of the line depending on the longest word
void lines(demo_products products2[], int row, int size)
{
    mvprintw(row, 10, "+--");
    for (int i = 0; i < check_longest_word(products2, size); ++i) {
        mvprintw(row, 12+i,"-");
    }
}

int render_results_view(WINDOW window, results_view_data_t *data) {
    double total_price1;
    double total_price2;
    char* word1 = products2[rand() % size].store;
    char* word2 = products2[rand() % size].store;
    int store_space1 = (17-(strlen(word1)+6))/2;
    int store_space2 = (17-(strlen(word2)+6))/2;

    mvprintw(0, 15+check_longest_word(products2, size)+store_space1, "Grocery list");

    //The store row
    lines(products2,1,size);
    mvprintw(1, 12+check_longest_word(products2, size), "+--------------------");
    mvprintw(1, 15+check_longest_word(products2, size)+store_space1, " %s (0) ", word1);
    mvprintw(1, 33+check_longest_word(products2, size), "+--------------------");
    mvprintw(1, 36+check_longest_word(products2, size)+store_space2, " %s (1) ", word2);
    mvprintw(1, 54+check_longest_word(products2, size), "+");

    //The name of the items, price and weight/amount row
    int i = 0;
    while (i < size-1) //Minus 1 because we don't want "q" to be there
    {
        mvprintw(2+i, 10, "|%s ", products2[i].product_name);
        mvprintw(2+i, 12 + check_longest_word(products2, size), "| %.2lf kr (%s) ", products2[i].price, products2[i].amount);
        mvprintw(2+i, 33 + check_longest_word(products2, size), "| %.2lf kr (%s) ", products2[i+1].price, products2[i].amount);
        mvprintw(2+i, 54 + check_longest_word(products2, size), "|");

        total_price1 = total_price1 + products2[i].price;
        total_price2 = total_price2 + products2[i+1].price;
        i++;
    }

    lines(products2,2+i,size);
    mvprintw(2+i, 12+check_longest_word(products2, size), "+--------------------+--------------------+");

    //The total price row
    mvprintw(3+i, 10, "|Total");
    mvprintw(3+i, 12 + check_longest_word(products2, size), "| %.2lf kr ", total_price1);
    mvprintw(3+i, 33 + check_longest_word(products2, size), "| %.2lf kr ", total_price2);
    mvprintw(3+i, 54 + check_longest_word(products2, size), "|");

    lines(products2,4+i,size);
    mvprintw(4+i, 12+check_longest_word(products2, size), "+--------------------+--------------------+");

    //The distance row
    mvprintw(5+i, 10, "|Distance");
    mvprintw(5+i, 12 + check_longest_word(products2, size), "| %d meters away ", products2[rand() % size].distance);
    mvprintw(5+i, 33 + check_longest_word(products2, size), "| %d meters away ", products2[rand() % size].distance);
    mvprintw(5+i, 54 + check_longest_word(products2, size), "|");

    lines(products2,6+i,size);
    mvprintw(6+i, 12+check_longest_word(products2, size), "+--------------------+--------------------+\n\n");
}