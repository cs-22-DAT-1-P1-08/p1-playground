#include "sorting_functions.h"
#include "api/coop_api.h"
#include "store.h"
#include <string.h>
#include "sorting_functions.h"
#include <curl/curl.h>
#include <stdlib.h>

//compares results to the new item found in the list
int cmp_item_price(item_t *a, item_t *b) {
    return get_item_price(a) > get_item_price(b);
}

item_t *find_cheapest_match(store_t *store, char *search_term) {
    dlist_node_t *item_node = store->items->head;
    item_t *results = NULL;
    while (item_node != NULL) {
        item_t *item = item_node->data;
        // TODO: make string comparison case insensitive
        if (item != NULL && strstr(item->name, search_term) != NULL) {
            // compares "results" with they knew item, which has the same string
            if (results == NULL || cmp_item_price(results, item)) {
                results = item;
            }
        }

        item_node = item_node->next;
    }
    //returns the result
    return results;
}

item_t *compare_item(store_t *store[], char *search_term, size_t length_of_store)
{
    item_t *cheapest_item = find_cheapest_match(store[0], search_term);
    for (int i = 1; i < length_of_store; i++) {
        item_t *temp_match = find_cheapest_match(store[i], search_term);
        if (cmp_item_price(cheapest_item, temp_match)) {
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

void mads_is_a_PDF_FILE(){
    char *nigger = "Mads Likes Even Bigger Cocks";
    char *woman_mads_like = " ";
    char* split_up = strtok(nigger, woman_mads_like);

    printf("%s\n", split_up);

    while(*split_up != 0){
        split_up = strtok(NULL, woman_mads_like);
        printf("%s\n", split_up);
    }
}


amount_t *find_amount(char *unit_amount) {
    amount_t *app = malloc(sizeof(amount_t));
    const char n[2] = " ";
        //for (int i = 0; i < input_of_amount; ++i) {
    char *a = strtok(unit_amount, n);
    app->amount = atof(a);
    a = strtok(NULL, n);
    app->unit_type = look_for_enumtype(a);
    printf("%lf\n\n%d", app->amount, app->unit_type);
   // }
    return app;
}
 int look_for_enumtype(char* str) {
    int amount = -1;
    if(strcmp(str, "GRAMS") == 0){
        amount = GRAMS;
    } else if(strcmp(str, "LITERS") == 0){
        amount = LITERS;
    } else if (strcmp(str, "PIECES") == 0){
        amount = PIECES;
    }
    return amount;
}




void print_item(store_t *store[], char *search_term[], size_t length_of_store, size_t input_of_product) {
    item_t *products =  different_items(store, search_term, length_of_store, input_of_product);
    printf("Your cheapest match is the following:\n");
    for(int i = 0; i < input_of_product; i++) {
        printf("%s: %lf DKK, %lf %d \n", products[i].name, get_item_price(&products[i]));
        printf("On the following product, you save exactly %lf DKK\n\n", products[i].price - get_item_price(&products[i]));
    }
    free(products);
}