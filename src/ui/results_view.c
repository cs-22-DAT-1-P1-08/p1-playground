#include "../util/asprintf.h" /* NOTE: this has to be placed *before* '#include <stdio.h>' */

#include "results_view.h"
#include "../sorting_functions.h"
#include "components/table.h"
#include "components/arrow_menu.h"
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

char* item_to_string(item_t *match, double *total_price) {
    if (match == NULL) {
        return strdup("N/A");
    }

    *total_price += match->price;
    amount_t *amount = match->amount;

    char amount_buf[50] = { 0 };
    if (amount)
        sprintf(amount_buf, " (%.2lf %s)", amount->amount, get_unit_name(amount->unit_type));

    return qasprintf( "%.2lf kr%s\n%s", match->price, amount_buf, match->name);
}

int render_results_view(WINDOW *window, results_view_data_t *data) {
    int i, j;

    /* Prepare data for table */
    int store_count = (int)data->store_count;

    double *store_totals = calloc(store_count, sizeof(double));

    char **shopping_list_arr = calloc(data->shopping_list->count, sizeof(char*));
    dlist_fill_array(data->shopping_list, (void **) shopping_list_arr);

    item_t *cheapest_items = different_items(data->stores, shopping_list_arr, store_count, data->shopping_list->count);
    double cheapest_items_total = 0;

    const int QUERY_COLUMN = 0;
    const int CHEAPEST_COLUMN = 1;
    const int OFFSET_LEFT = 2;
    const int TABLE_WIDTH = store_count + OFFSET_LEFT;

    /* Create table */
    table_t *table = init_table();
    table->padding_x = 1;

    /* Add header row */
    table_row_t *header_row = table_add_empty_row(table, ROW_BOLD | ROW_ACENTER, TABLE_WIDTH);
    table_row_update_field(header_row, CHEAPEST_COLUMN, "Cheapest options");
    for (i = 0; i < store_count; i++)
        table_row_update_field(header_row, i + OFFSET_LEFT, data->stores[i]->name);

    /* Add product rows */
    dlist_node_t *node;
    for (i = 0, node = data->shopping_list->head; node != NULL; i++, node = node->next) {
        char* list_entry = node->data;

        table_row_t *row = table_add_empty_row(table, ROW_SIMPLE, store_count + OFFSET_LEFT);
        table_row_update_field(row, QUERY_COLUMN, list_entry);
        table_row_update_field(row, CHEAPEST_COLUMN, item_to_string(&cheapest_items[i], &cheapest_items_total));

        for (j = 0; j < store_count; j++) {
            item_t *match = find_cheapest_match(data->stores[j], list_entry);
            table_row_update_field(row, j + OFFSET_LEFT, item_to_string(match, &store_totals[j]));
        }
    }

    /* Add totals row */
    table_row_t *totals_row = table_add_empty_row(table, ROW_BOLD, TABLE_WIDTH);
    table_row_update_field(totals_row, QUERY_COLUMN, "Total");
    table_row_update_field(totals_row, CHEAPEST_COLUMN, qasprintf("%.2lf kr", cheapest_items_total));
    for (i = 0; i < store_count; i++)
        table_row_update_field(totals_row, i + OFFSET_LEFT, qasprintf("%.2lf kr", store_totals[i]));

    render_table(window, table);

    /* Add location row (if user address is set) */
    if (data->home != NULL && !is_addr_empty(data->home)) {
        wprintw(window, "\n\nFetching location information...");
        wrefresh(window);

        table_row_t *location_row = table_add_empty_row(table, ROW_DEFAULT, TABLE_WIDTH);
        table_row_update_field(location_row, QUERY_COLUMN, "Travel time (car)\nOne way (Round-trip)");

        dlist_t *all_places_list = calloc(1, sizeof(dlist_t));
        dlist_add(all_places_list, data->home);

        for (i = 0; i < store_count; i++) {
            dlist_add(all_places_list, data->stores[i]->location);

            location_t *places[] = { data->home, data->stores[i]->location, data->home };
            int *arr = route_time(places, "car", data->here_api_key, 3, 1);

            table_row_update_field(location_row, i + OFFSET_LEFT, qasprintf("%.2lf minutes\n(%.2lf minutes)", (arr[0]-arr[2]) / 60.0, arr[0] / 60.0));
        }
        dlist_add(all_places_list, data->home);

        location_t **all_places = calloc(all_places_list->count, sizeof(location_t*));
        dlist_fill_array(all_places_list, (void **) all_places);

        int places_n = (int)all_places_list->count;
        int *arr2 = route_time(all_places, "car", data->here_api_key, places_n, 1);
        table_row_update_field(location_row, CHEAPEST_COLUMN, qasprintf("%.2lf minutes\n(%.2lf minutes)", (arr2[0] - arr2[places_n - 1]) / 60.0, arr2[0] / 60.0));

        wclear(window);
        render_table(window, table);
    }

    wrefresh(window);

    menu_item_t menu_items[] = {
            {"Edit shopping list"},
            {"Return to main menu"}
    };

    wprintw(window, "\n\n");
    int selected = render_menu(window, menu_items, sizeof(menu_items) / sizeof(menu_items[0]));

    return selected;
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