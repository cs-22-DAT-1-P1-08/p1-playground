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

    *total_price += get_item_price(match);
    amount_t *amount = match->amount;

    char amount_buf[50] = { 0 };
    if (amount)
        sprintf(amount_buf, " (%.2lf %s)", amount->amount, get_unit_name(amount->unit_type));

    char offer_buf[50] = {0};
    if (match->offer != NULL)
        sprintf(offer_buf, " (save %.2lf kr)", match->price - get_item_price(match));

    return qasprintf( "%.2lf kr%s%s\n%s", get_item_price(match), offer_buf, amount_buf, match->name);
}

int render_results_view(WINDOW *window, results_view_data_t *data) {
    int i, j;

    /* Prepare data for table */
    int store_count = (int)data->store_count;

    double *store_totals = calloc(store_count, sizeof(double));

    char **shopping_list_arr = calloc(data->shopping_list->count, sizeof(char*));
    dlist_fill_array(data->shopping_list, (void **) shopping_list_arr);

    item_t **cheapest_items = different_items(data->stores, shopping_list_arr, store_count, data->shopping_list->count);
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
        table_row_update_field(row, CHEAPEST_COLUMN, item_to_string(cheapest_items[i], &cheapest_items_total));

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

        int places_n = (int)all_places_list->count;
        location_t **all_places = calloc(places_n, sizeof(location_t*));
        dlist_fill_array(all_places_list, (void **) all_places);
        dlist_free(all_places_list);

        int *arr = route_time(all_places, "car", data->here_api_key, places_n, 1);
        table_row_update_field(location_row, CHEAPEST_COLUMN, qasprintf("%.2lf minutes\n(%.2lf minutes)", (arr[0] - arr[places_n - 1]) / 60.0, arr[0] / 60.0));
        free(all_places);

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

    free_table(table);
    return selected;
}