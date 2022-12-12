#ifndef STORE_H
#define STORE_H

#include "api/coop_api.h"
#include "api/tjek_api.h"
#include "api/location_api.h"

enum StoreType {
    COOP,
    SALLING
};

typedef struct {
    char* name;
    location_t* location;
    catalog_offers_t* offers;
    dlist_t* products;
    catalog_info_t* catalog_info;
} store_t;

store_t* get_coop_store(char* store_name, char* store_id, char* dealer_id);

/**
 * Fills store location information based on a home location.
 * @param home target location, find store close to this location.
 */
void fill_nearest_store(store_t *store, location_t *home);

void free_store(store_t *store);

#endif


