#include "sorting_functions.h"
#include "api/coop_api.h"
#include "store.h"
#include <string.h>
#include "sorting_functions.h"
#include <curl/curl.h>

//compares results to the new item found in the list
int cmp_item_price(item_t *a, item_t *b) {
    // TODO: Include offer prices
    return (int)(a->price - b->price);
}

item_t *find_cheapest_match(store_t *store, char *search_term) {
    item_t *results;
    dlist_node_t *item_node = store->items->head;
    while (item_node != NULL) {
        item_t *item = item_node->data;
        // TODO: make string comparison case insensitive
        if (item != NULL && strstr(item->name, search_term) != NULL) {
            // compares "results" with they knew item, which has the same string
            if (cmp_item_price(results, item)) {
                *results = *item;
            }
        }
        item_node = item_node->next;
    }
    //returns the result
    return results;
}

item_t *compare_item(store_t *store, char *search_term, size_t length_of_store)
{
    item_t *cheapest_item = find_cheapest_match(&store[0], search_term);

    for (int i = 1; i < length_of_store; ++i) {
            item_t *temp_match = find_cheapest_match(&store[i], search_term);
            if (cmp_item_price(cheapest_item, temp_match)) {
                *cheapest_item = *temp_match;
            }
    }
    return cheapest_item;
}

void print_items(store_t *store, char *search_term)
{

    printf("%s: %lf\n", find_cheapest_match(store, search_term)->name,
                        find_cheapest_match(store, search_term)->price);
    //printf("%s: %lf", compare_stores(stores, search_term).item, compare_stores(stores).price);
}