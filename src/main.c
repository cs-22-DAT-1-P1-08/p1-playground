#include <stdio.h>
#include "api/coop_api.h"
#include "store.h"
#include "api/tjek_api.h"
#include "sorting_functions.h"
#include <curl/curl.h>

void print_products(store_t *store);
void coop_api_temp(void);
void tjek_api_temp();
void sorting_temp();
void search_prototype(store_t *daglibrugsen, store_t *coop365);

int main() {
    printf("Started program...\n");
    store_t *daglibrugsen = get_coop_store("1290", DB_DEALER_ID);
    store_t *coop365 = get_coop_store("24165", COOP365_DEALER_ID);

    print_products(daglibrugsen);
    //search_prototype(daglibrugsen, coop365);

    free_store(daglibrugsen);
    free_store(coop365);
    return 0;
}

void print_products(store_t *store) {
    dlist_node_t *item_node = store->items->head;
    while (item_node != NULL) {
        item_t *item = item_node->data;
        if (item == NULL)
            continue;

        printf("[%s] %s (%.2lf kr)", item->ean, item->name, item->price);
        if (item->amount != NULL)
            printf(" %.2lf %s", item->amount->amount, get_unit_name(item->amount->unit_type));
        if (item->offer != NULL)
            printf("\n ---------- TILBUD /\\: %.2lf kr ------------- ", item->offer->group->price);
        printf("\n");

        item_node = item_node->next;
    }
}

void search_prototype(store_t *daglibrugsen, store_t *coop365) {
    char temp_search_term[100];
    memset(temp_search_term, 0, 100);
    printf("Enter search term: ");
    scanf("%s", temp_search_term);

    printf("\n------------------------\n");
    printf("Searching for %s: \n", temp_search_term);
    item_t *db_cheapest = find_cheapest_match(daglibrugsen, temp_search_term);
    if (db_cheapest != NULL)
        printf("Dagli'Brugsen: %s (%.2lf kr)\n", db_cheapest->name, db_cheapest->price);
    else printf("Dagli'Brugsen: Not found");

    item_t *cheapest365 = find_cheapest_match(coop365, temp_search_term);
    if (db_cheapest != NULL && db_cheapest->name != NULL)
        printf("Coop 365: %s (%.2lf kr)\n", cheapest365->name, cheapest365->price);
    else printf("Coop 365: Not found");
}

void coop_api_temp(void) {
    /* Daglibrugsen */
    dlist_t* list_dagligbrugsen = coop_get_items("1290");

    /* coop 365 */
    dlist_t* list_coop365 = coop_get_items("24165");

    dlist_node_t *item_node = list_dagligbrugsen->head;
    while (item_node != NULL) {
        item_t *item = item_node->data;
        if (item->amount != NULL) {
            printf("%s: %.2lf %s\n", item->name, item->amount->amount, get_unit_name(item->amount->unit_type));
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

    dlist_node_t *offer_node = catalog_offers->items->head;
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
