#ifndef TJEK_API_H
#define TJEK_API_H

#include "../util/dlist.h"
#include <time.h>

#define COOP365_DEALER_ID "DWZE1w"
#define DB_DEALER_ID "d311fg"

typedef struct catalog_info_t {
    char *id;
    char *dealer_id;
    int offer_count;
    time_t start_time;
    time_t end_time;
} catalog_info_t;

typedef struct offer_group_t {
    char *id;
    char *heading;
    double price;
    dlist_t *offers;
} offer_group_t;

typedef struct offer_t {
    offer_group_t *group;
    char *ean;
    char *title;
} offer_t;

typedef struct catalog_offers_t {
    dlist_t *offer_groups;
    dlist_t *offers;
} catalog_offers_t;
/**
 * Fetches active catalog info from a given dealer using the Tjek API
 */
catalog_info_t* get_catalog_info(char *dealer_id);
void free_catalog_info(catalog_info_t *info);

// WIP
catalog_offers_t* get_catalog_offers(catalog_info_t *catalog_info);
// WIP
catalog_offers_t* get_catalog_products(catalog_info_t *catalog_info, catalog_offers_t *catalog_offers);
void free_offer(offer_t *offer);
void free_offer_group(offer_group_t *offer_group);
void free_catalog_offers(catalog_offers_t *catalog_offers);


#endif