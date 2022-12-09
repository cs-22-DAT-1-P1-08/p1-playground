// C implementation of Radix Sort
#include "util/dlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "api/coop_api.h"
#include "store.h"

int cmp_item_price(item_t *a, item_t *b);

item_t *find_cheapest_match(store_t *store, char *search_term);

item_t *compare_item(store_t *store[], char *search_term, size_t length_of_store);

item_t *different_items(store_t *store[], char *search_term[], size_t length_of_store, size_t length_of_input);

void print_item(store_t *store[], char *search_term[], size_t length_of_store, size_t length_of_input);

#ifndef P1_PLAYGROUND_SORTING_FUNCTIONS_H
#define P1_PLAYGROUND_SORTING_FUNCTIONS_H

#endif //P1_PLAYGROUND_SORTING_FUNCTIONS_H
