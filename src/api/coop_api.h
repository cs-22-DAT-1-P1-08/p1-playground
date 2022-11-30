//
// Created by carl on 28/11/2022.
//

#ifndef P1_PLAYGROUND_COOP_API_H
#define P1_PLAYGROUND_COOP_API_H
#include "../util/dlist.h"

/**
 * Retrieves all items from a given coop store.
 * @param store_id store kardex number string
 * @return list of items
 */
dlist_t* coop_get_items(char* store_id);

/**
 * Struct for items
 */
typedef struct item_t{
    char* ean;
    double price;
    char* name;
    char* amount;
} item_t;


void free_item(item_t* item);

#endif //P1_PLAYGROUND_COOP_API_H
