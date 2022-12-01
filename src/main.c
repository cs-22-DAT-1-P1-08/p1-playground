#include <stdio.h>
#include "api/coop_api.h"
#include <stdlib.h>
#include "api/tjek_api.h"
#include <curl/curl.h>
#include <locale.h>
#include <wchar.h>

void coop_api_temp(void);
void tjek_api_temp();


int main() {
    coop_api_temp();
    return 0;
}

void coop_api_temp(void){
    amount_t *test = find_amount_from_string("ENTRECOTE 350G");
    if (test == NULL) {
        fprintf(stderr, "null");
        exit(EXIT_FAILURE);
    }
    printf("%lf", test->amount);

    return;
    /* Daglibrugsen */
    dlist_t* list_dagligbrugsen = coop_get_items("1290");

    /* coop 365 */
    dlist_t* list_coop365 = coop_get_items("24165");

    dlist_node_t *item_node = list_dagligbrugsen->head;
    while (item_node != NULL) {
        item_t *item = item_node->data;
        if (item->amount != NULL) {
            printf("%s: %.2lf %d\n", item->name, item->amount->amount, item->amount->unit_type);
        }

        item_node = item_node->next;
    }


    dlist_free_all(list_dagligbrugsen, (void (*)(void *)) free_item);
    dlist_free_all(list_coop365, (void (*)(void *)) free_item);
}

void tjek_api_temp() {
    catalog_info_t *db_catalog_info = get_catalog_info(COOP365_DEALER_ID);
    printf("%s: %d\n", db_catalog_info->id, db_catalog_info->offer_count);

    catalog_offers_t *catalog_offers = get_catalog_offers(db_catalog_info);

    dlist_node_t *offer_node = catalog_offers->offers->head;
    while (offer_node != NULL) {
        offer_t *offer = (offer_t*)offer_node->data;
        if (offer->group == NULL) {
            printf("Something is rotten in the state of Denmark...\n");
            offer_node = offer_node->next;
            continue;
        }
        printf("%.2lf kr, %s [%s]\n", offer->group->price, offer->title, offer->ean);
        offer_node = offer_node->next;
    }

    free_catalog_offers(catalog_offers);
    free_catalog_info(db_catalog_info);
}