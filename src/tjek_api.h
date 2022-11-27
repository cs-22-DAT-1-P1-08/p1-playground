#ifndef TJEK_API_H
#define TJEK_API_H

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

/**
 * Fetches active catalog info from a given dealer using the Tjek API
 */
catalog_info_t* get_catalog_info(char *dealer_id);
void free_catalog_info(catalog_info_t *info);

// WIP
void get_catalog_offers(catalog_info_t *catalog_info);

#endif