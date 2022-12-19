#include <stdio.h>
#include "sorting_functions.h"
#include "api/coop_api.h"
#include "store.h"
#include <string.h>
#include "sorting_functions.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <math.h>

char* to_lowercase(char* input_str) {
    char* result = calloc(strlen(input_str) + 1, sizeof(char));
    for (int i = 0; i < strlen(input_str); i++) {
        result[i] = (char)tolower(input_str[i]);
    }
    return result;
}

//compares results to the new item found in the list
int cmp_item_price(item_t *a, item_t *b) {
    // It's not possible to compare amount between pieces and other unit types
    if (a->amount == NULL || b->amount == NULL ||
        a->amount->unit_type == PIECES ^ b->amount->unit_type == PIECES)
        return get_item_price(a) > get_item_price(b);

    // Compare same amount unit
    if (a->amount->unit_type == b->amount->unit_type)
        return (get_item_price(a) / a->amount->amount) >
               (get_item_price(b) / b->amount->amount);

    // Convert liters to grams
    if (a->amount->unit_type == LITERS && b->amount->unit_type == GRAMS)
        return (get_item_price(a) / (a->amount->amount * 1000)) >
               (get_item_price(b) / b->amount->amount);

    // Reverse
    if (a->amount->unit_type == GRAMS && b->amount->unit_type == LITERS)
        return (get_item_price(a) / a->amount->amount) >
               (get_item_price(b) / b->amount->amount * 1000);

    // Fallback scenario if specific case is not implemented
    return get_item_price(a) > get_item_price(b);
}

dlist_t *split_input(char *search_term) {
    char delimiter[] = " ";
    char *temp_str = strtok(search_term, delimiter);
    dlist_t *str_output = calloc(1, sizeof(dlist_t));
    dlist_add(str_output, temp_str);
    int i = 0;
    while(temp_str != NULL){
        temp_str = strtok(NULL, delimiter);
        if(temp_str != NULL){
            dlist_add(str_output, temp_str);
            i++;
        }
    }
    return str_output;
}

int cmp_user_friendliness(char *item, dlist_t *search_term){
    int results = 0;
    for (int i = 0; i < search_term->count; ++i) {
        char *temp_term = (char*) (dlist_get_at(search_term, i)->data);
        if(strstr(item, temp_term)) {
            results++;
        }
    }

    return results;
}

item_t *find_cheapest_match(store_t *store, char *search_term) {
    char* lower_search_term = to_lowercase(search_term);
    dlist_t *str_output = split_input(lower_search_term);

    int search_hits = 1;

    item_t *results = NULL;
    dlist_node_t *item_node = store->products->head;
    while (item_node != NULL) {

        item_t *item = item_node->data;
        char* item_name = to_lowercase(item->name);

        int temp_search_hits = cmp_user_friendliness(item_name, str_output);
        if (temp_search_hits >= search_hits) {
            search_hits = temp_search_hits;

            // compares "results" with the new item, which has the same string parts
            if (results == NULL || cmp_item_price(results, item))
                results = item;
        }

        free(item_name);
        item_node = item_node->next;
    }

    dlist_free(str_output);
    free(lower_search_term);
    return results;
}

item_t *compare_item(store_t *store[], char *search_term, size_t length_of_store) {
    item_t *cheapest_item = find_cheapest_match(store[0], search_term);
    for (int i = 1; i < length_of_store; i++) {
        item_t *temp_match = find_cheapest_match(store[i], search_term);
        if (temp_match != NULL && (cheapest_item == NULL || cmp_item_price(cheapest_item, temp_match))) {
            cheapest_item = temp_match;
        }

    }
    return cheapest_item;
}

item_t *different_items(store_t *store[], char *search_term[], size_t length_of_store, size_t length_of_input) {
    item_t *products = malloc(length_of_input * sizeof(item_t));
    for (int j = 0; j < length_of_input; ++j) {
        item_t *cheapest_specific_item = compare_item(store, search_term[j], length_of_store);
        products[j] = *cheapest_specific_item;
    }
    return products;
}

int cmp_amount(item_t item, amount_t UI) {
    if (item.amount == NULL) {
        return (int) UI.amount;
    }
    int n = ceil(UI.amount / item.amount->amount);
    if (n <= 1) {
        return 1;
    } else
        return n;
}

void print_item(store_t *store[], char *search_term[], char *amounts[], size_t length_of_store, size_t input_of_product) {
    item_t *products =  different_items(store, search_term, length_of_store, input_of_product);
    printf("Your cheapest match is the following:\n");
    for(int i = 0; i < input_of_product; i++) {
        amount_t *amount = find_amount_from_string(amounts[i]);
        if (amount == NULL) {
            fprintf(stderr, "sus %s: %s\n", products[i].name, amounts[i]);
            continue;
        }
        printf("You need %d x %.2lf %s %s , where each product will cost %.2lf DKK\n",
               cmp_amount(products[i],*amount),
               (products[i].amount != NULL) ? products[i].amount->amount : 1,
               (products[i].amount != NULL) ? get_unit_name(products[i].amount->unit_type) : get_unit_name(amount->unit_type),
               products[i].name,
               get_item_price(&products[i]));
        printf("On the following product, you'll save exactly %lf DKK\n\n", products[i].price - get_item_price(&products[i]));
    }
    free(products);
}