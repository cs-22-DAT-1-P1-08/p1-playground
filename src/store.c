#include "store.h"
#include <stdlib.h>
#include <string.h>
#include "api/coop_api.h"
#include "api/tjek_api.h"
#include "api/location_api.h"
#include "curl/curl.h"

store_t link_offer_data(store_t* store) {
    dlist_node_t *item_node = store->products->head;
    while (item_node != NULL) {
        item_t *item = item_node->data;
        if (item == NULL) {
            item_node = item_node->next;
            continue;
        }

        dlist_node_t *offer_node = store->offers->products->head;
        while (offer_node != NULL) {
            offer_t *offer = offer_node->data;
            if (offer != NULL && strcmp(offer->ean, item->ean) == 0)
                item->offer = offer;
            offer_node = offer_node->next;
        }

        item_node = item_node->next;
    }
}

store_t* get_coop_store(char* store_name, char* store_id, char* dealer_id) {
    store_t *store = calloc(1, sizeof(store_t));

    /* Fill store name */
    store->name = calloc(strlen(store_name) + 1, sizeof(char));
    strcpy(store->name, store_name);

    /* Fill assortment data from Coop API */
    store->products = coop_get_items(store_id);

    /* Fill offer data from Tjek API */
    store->catalog_info = get_catalog_info(dealer_id);
    store->offers = get_catalog_offers(store->catalog_info);

    /* Link product and offer data */
    link_offer_data(store);

    return store;
}

void fill_nearest_store(store_t *store, geocode *home) {
    if (store->location == NULL) {
        store->location = malloc(sizeof(geocode));
    }

    initialize_geocode(store->location);

    char* escaped_name = curl_escape(store->name, strlen(store->name));
    strcpy(store->location->place_name, escaped_name);
    strcpy(store->location->city, home->city);
    curl_free(escaped_name);
}

void free_store(store_t *store) {
    if (store == NULL)
        return;

    if (store->name != NULL)
        free(store->name);

    if (store->location != NULL)
        free(store->location);

    if (store->catalog_info != NULL)
        free_catalog_info(store->catalog_info);

    if (store->offers != NULL)
        free_catalog_offers(store->offers);

    if (store->products != NULL)
        dlist_free_all(store->products, (void (*)(void *)) free_item);

    free(store);
}