#include "results_view.h"
#include "../sorting_functions.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>

//a struct for fake data
typedef struct {
    char product_name[30];
    char store[20];
    char amount[10];
    double price;
    int distance;
} demo_products;

//Checks for the longest product name and changes the size of the box
int check_longest_word(item_t *products2[], int size)
{
    int result;
    int maxLength = 9;
    char* word;

    for (int i = 0; i < size; ++i)
    {
        word = products2[i]->name;
        if (strlen(word) > maxLength)
            maxLength = strlen(word);

        result = maxLength;
    }

    return result;
}

//Changes the size of the line depending on the longest word
void lines(item_t *products2[], int row, int size)
{
    mvprintw(row, 10, "+--");
    for (int i = 0; i < check_longest_word(products2, size); ++i) {
        mvprintw(row, 12+i,"-");
    }
}

int render_results_view(WINDOW *window, results_view_data_t *data) {
    double total_price1;
    double total_price2;

    char* store1_name = data->stores[0]->name;
    char* store2_name = data->stores[1]->name;

    int size = (int)data->shopping_list->count;
    char **shopping_list = calloc(size, sizeof(char*));
    dlist_fill_array(data->shopping_list, (void **) shopping_list);

    item_t **products2 = calloc(size * 2, sizeof (item_t*));
    for (int i = 0; i < size * 2; i++) {
        products2[i] = find_cheapest_match(data->stores[i % 2], shopping_list[i / 2]);
    }


    int store_space1 = (int)((17-(strlen(store1_name) + 6)) / 2);
    int store_space2 = (int)((17-(strlen(store2_name) + 6)) / 2);

    int longest_word_len = check_longest_word(products2, size);

    mvprintw(0, 15+longest_word_len+store_space1, "Grocery list");

    //The store row
    lines(products2,1,size);
    mvprintw(1, 12+longest_word_len, "+--------------------");
    mvprintw(1, 15+longest_word_len+store_space1, " %s (0) ", store1_name);
    mvprintw(1, 33+longest_word_len, "+--------------------");
    mvprintw(1, 36+longest_word_len+store_space2, " %s (1) ", store2_name);
    mvprintw(1, 54+longest_word_len, "+");

    //The name of the items, price and weight/amount row
    int i = 0;
    while (i < size * 2) //Minus 1 because we don't want "q" to be there
    {
        mvprintw(2+i, 10, "|%s ", shopping_list[i / 2]);

        if (products2[i] != NULL)
            mvprintw(2+i, 12 + longest_word_len, "| %.2lf kr (%s) ", get_item_price(products2[i]), get_amount_string(products2[i]->amount));
        else
            mvprintw(2+i, 12 + longest_word_len, " | N/A ");

        if (products2[i + 1] != NULL)
            mvprintw(2+i, 33 + longest_word_len, "| %.2lf kr (%s) ", get_item_price(products2[i + 1]), get_amount_string(products2[i + 1]->amount));
        else
            mvprintw(2+i, 33 + longest_word_len, " | N/A ");

        mvprintw(2+i, 54 + longest_word_len, "|");

        total_price1 = total_price1 + get_item_price(products2[i]);
        total_price2 = total_price2 + get_item_price(products2[i+1]);
        i += 2;
    }

    lines(products2,2+i,size);
    mvprintw(2+i, 12+longest_word_len, "+--------------------+--------------------+");

    //The total price row
    mvprintw(3+i, 10, "|Total");
    mvprintw(3+i, 12 + longest_word_len, "| %.2lf kr ", total_price1);
    mvprintw(3+i, 33 + longest_word_len, "| %.2lf kr ", total_price2);
    mvprintw(3+i, 54 + longest_word_len, "|");

    lines(products2,4+i,size);
    mvprintw(4+i, 12+longest_word_len, "+--------------------+--------------------+");

    //The distance row
    mvprintw(5+i, 10, "|Distance");
    mvprintw(5+i, 12 + longest_word_len, "| %d meters away ", -1);//products2[rand() % size].distance);
    mvprintw(5+i, 33 + longest_word_len, "| %d meters away ", -2); //products2[rand() % size].distance);
    mvprintw(5+i, 54 + longest_word_len, "|");

    lines(products2,6+i,size);
    mvprintw(6+i, 12+longest_word_len, "+--------------------+--------------------+\n\n");
}