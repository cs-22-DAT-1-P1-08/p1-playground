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

item_t amount_per_price(store_t *store[], char *unit_amount[], size_t length_of_store, size_t input_of_amount);

//amount_t *find_amount(char *unit_amount[], size_t input_of_amount);
amount_t *find_amount(char *unit_amount);

void mads_is_a_PDF_FILE();

int look_for_enumtype(char* str);

void print_item(store_t *store[], char *search_term[], size_t length_of_store, size_t input_of_product);

#ifndef P1_PLAYGROUND_SORTING_FUNCTIONS_H
#define P1_PLAYGROUND_SORTING_FUNCTIONS_H

#endif //P1_PLAYGROUND_SORTING_FUNCTIONS_H
