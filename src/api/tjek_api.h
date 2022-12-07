#ifndef TJEK_API_H
#define TJEK_API_H

#include <time.h>
#include "../util/dlist.h"

#define COOP365_DEALER_ID "DWZE1w"
#define DB_DEALER_ID "d311fg"

/**
 * Stores information from Tjek API about grocery catalog (sales paper)
 */
typedef struct catalog_info_t {
    char *id;
    char *dealer_id;
    int offer_count;
    time_t start_time;
    time_t end_time;
} catalog_info_t;


/**
 * Collection of products with common campaign/offer
 */
typedef struct offer_group_t {
    char *id;
    char *heading;
    double price;
    dlist_t *offers;
} offer_group_t;

/**
 * Specific product offer within a campaign/offer
 */
typedef struct offer_t {
    offer_group_t *group;
    char *ean;
    char *title;
} offer_t;

/**
 * All offers in a catalog (sales paper)
 */
typedef struct catalog_offers_t {
    dlist_t *offer_groups;
    dlist_t *products;
} catalog_offers_t;

/**
 * Fetches active catalog info from a given dealer using the Tjek API
 */
catalog_info_t* get_catalog_info(char *dealer_id);
void free_catalog_info(catalog_info_t *info);

/**
 * Fetches all offers from a given catalog (sales paper) using Tjek API
 * @param catalog_info Catalog to fetch offers from (must contain valid id & dealer_id)
 * @return struct containing offers and offer groups/collections
 */
catalog_offers_t* get_catalog_offers(catalog_info_t *catalog_info);

void free_offer(offer_t *offer);
void free_offer_group(offer_group_t *offer_group);
void free_catalog_offers(catalog_offers_t *catalog_offers);


#endif