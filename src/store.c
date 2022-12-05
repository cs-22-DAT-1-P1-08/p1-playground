#include "store.h"
#include <stdlib.h>
#include "api/coop_api.h"
#include "api/tjek_api.h"

store_t* get_coop_store(char* store_id, char* dealer_id) {
    store_t *store = calloc(1, sizeof(store_t));
    store->products = coop_get_items(store_id);
    store->catalog_info = get_catalog_info(dealer_id);
    store->offers = get_catalog_offers(store->catalog_info);
    return store;
}

void free_store(store_t *store) {
    if (store == NULL)
        return;

    if (store->catalog_info != NULL)
        free_catalog_info(store->catalog_info);

    if (store->offers != NULL)
        free_catalog_offers(store->offers);

    if (store->products != NULL)
        dlist_free_all(store->products, (void (*)(void *)) free_item);

    free(store);
}