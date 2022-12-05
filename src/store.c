#include "store.h"
#include <stdlib.h>
#include <string.h>
#include "api/coop_api.h"
#include "api/tjek_api.h"

store_t link_offer_data(store_t* store) {
    dlist_node_t *item_node = store->products->head;
    while (item_node != NULL) {
        item_t *item = item_node->data;
        if (item == NULL)
            continue;

        dlist_node_t *offer_node = store->offers->offers->head;
        while (offer_node != NULL) {
            offer_t *offer = offer_node->data;
            if (offer != NULL && strcmp(offer->ean, item->ean) == 0)
                item->offer = offer;
            offer_node = offer_node->next;
        }

        item_node = item_node->next;
    }
}

store_t* get_coop_store(char* store_id, char* dealer_id) {
    store_t *store = calloc(1, sizeof(store_t));
    store->products = coop_get_items(store_id);
    store->catalog_info = get_catalog_info(dealer_id);
    store->offers = get_catalog_offers(store->catalog_info);
    link_offer_data(store);
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