#include <stdio.h>
#include <stdlib.h>
#include "api/tjek_api.h"
#include <curl/curl.h>
#include <locale.h>

void tjek_api_temp() {
    setlocale(LC_ALL, "");
    catalog_info_t *db_catalog_info = get_catalog_info(COOP365_DEALER_ID);
    printf("%s: %d\n", db_catalog_info->id, db_catalog_info->offer_count);

    catalog_offers_t *catalog_offers = get_catalog_offers(db_catalog_info);

    dlist_node_t *offer_node = catalog_offers->offers->head;
    while (offer_node != NULL) {
        offer_t *offer = (offer_t*)offer_node->data;
        printf("%lf kr, %s [%s]", offer->group->price, offer->title, offer->ean);
        offer_node = offer_node->next;
    }
    //get_catalog_products(db_catalog_info);

    free_catalog_info(db_catalog_info);
}

int main() {
    tjek_api_temp();

    return 0;
}

