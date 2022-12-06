#ifndef STORE_H
#define STORE_H

#include "api/coop_api.h"
#include "api/tjek_api.h"

enum StoreType {
    COOP,
    SALLING
};

typedef struct store_t {
    char* name;
    catalog_offers_t* offers;
    dlist_t* products;
    catalog_info_t* catalog_info;
} store_t;

store_t* get_coop_store(char* store_name, char* store_id, char* dealer_id);
void free_store(store_t *store);

#endif


