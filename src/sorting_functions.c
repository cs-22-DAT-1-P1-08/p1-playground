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

item_t find_cheapest_match(store_t *store, char* search_term) {
    item_t results;
    dlist_node_t *item_node = store->items->head;
    while (item_node != NULL) {
        item_t *item = item_node->data;
        // TODO: make string comparison case insensitive
        if (item != NULL && strstr(item->name, search_term) != NULL) {
            // compares "results" with they new item, which has the same string
            if (cmp_item_price(&results, item)) {
                results = *item;
            }
        }
        item_node = item_node->next;
    }
    //returns the result
    return results;
}