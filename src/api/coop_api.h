//
// Created by carl on 28/11/2022.
//

#ifndef P1_PLAYGROUND_COOP_API_H
#define P1_PLAYGROUND_COOP_API_H
#include "../util/dlist.h"
#include "tjek_api.h"

enum AmountUnit {
    GRAMS,
    LITERS,
    PIECES
};

typedef struct{
    double amount;
    enum AmountUnit unit_type;
} amount_t;

amount_t* find_amount_from_string(char* input_str);
char* get_amount_string(amount_t *amount);
const char* get_unit_name(enum AmountUnit unit);

/**
 * Retrieves all items from a given coop store.
 * @param store_id store kardex number string
 * @return list of items
 */
dlist_t* coop_get_items(char* store_id);

/**
 * Struct for items
 */

typedef struct item_t {
    char* ean;
    double price;
    char* name;
    char* details;
    amount_t* amount;
    offer_t* offer;
} item_t;


void free_item(item_t* item);
double get_item_price(item_t* item);

#endif //P1_PLAYGROUND_COOP_API_H
