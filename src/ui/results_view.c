#include "results_view.h"
#include "../sorting_functions.h"
#include "components/table.h"
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

int render_results_view(WINDOW *window, results_view_data_t *data) {
    table_t *table = init_table();
    table->padding_x = 1;

    int i, j;
    int store_count = (int)data->store_count;

    /* Add header rows */
    char **header_row = calloc(store_count + 1, sizeof(char*));
    for (i = 0; i < store_count; i++)
        header_row[i + 1] = data->stores[i]->name;
    table_add_row_array(table, ROW_BOLD | ROW_ACENTER, store_count + 1, header_row);
    free(header_row);

    /* Create store total price array */
    double *store_totals = calloc(store_count, sizeof(double));

    /* Add product rows */
    dlist_node_t *node;
    for (i = 0, node = data->shopping_list->head; node != NULL; i++, node = node->next) {
        char* list_entry = node->data;
        char** row_data = calloc(store_count + 1, sizeof(char*));
        row_data[0] = strdup(list_entry);

        for (j = 0; j < store_count; j++) {
            item_t *match = find_cheapest_match(data->stores[j], list_entry);

            if (match == NULL) {
                row_data[j + 1] = strdup("N/A");
                continue;
            }

            store_totals[j] += match->price;
            amount_t *amount = match->amount;

            char amount_buf[50] = { 0 };
            if (amount)
                sprintf(amount_buf, " (%.2lf %s)", amount->amount, get_unit_name(amount->unit_type));

            char result_buf[255] = { 0 };
            sprintf(result_buf, "%.2lf kr%s\n%s", match->price, amount_buf, match->name);

            row_data[j + 1] = strdup(result_buf);
        }

        table_add_row_array(table, ROW_SIMPLE, store_count + 1, row_data);
        free(row_data);
    }

    /* Add total row */
    char **total_row = calloc(store_count + 1, sizeof(char*));
    total_row[0] = strdup("Total");
    for (i = 0; i < store_count; i++) {
        char total_buf[50] = { 0 };
        sprintf(total_buf, "%.2lf kr", store_totals[i]);;
        total_row[i + 1] = strdup(total_buf);
    }
    table_add_row_array(table, ROW_BOLD, store_count + 1, total_row);
    free(total_row);

    render_table(window, table);
    wrefresh(window);

    return 0;
}

/*

//Checks for the longest product name and changes the size of the box
int check_longest_word(item_t *products2[], int size)
{
    int result = 0;
    int maxLength = 9;
    char* word = "";

    for (int i = 0; i < size; ++i)
    {
        word = (products2[i] != NULL) ? products2[i]->name : "";
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
    double total_price1 = 0;
    double total_price2 = 0;

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

        if (products2[i] != NULL) {
            mvprintw(2+i, 12 + longest_word_len, "| %.2lf kr (%s) ", get_item_price(products2[i]), get_amount_string(products2[i]->amount));
            total_price1 = total_price1 + get_item_price(products2[i]);

        }
        else {
            mvprintw(2+i, 12 + longest_word_len, " | N/A ");
        }

        if (products2[i + 1] != NULL)  {
            mvprintw(2+i, 33 + longest_word_len, "| %.2lf kr (%s) ", get_item_price(products2[i + 1]), get_amount_string(products2[i + 1]->amount));
            total_price2 = total_price2 + get_item_price(products2[i+1]);

        }
        else {
            mvprintw(2+i, 33 + longest_word_len, " | N/A ");
        }

        mvprintw(2+i, 54 + longest_word_len, "|");

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
}*/