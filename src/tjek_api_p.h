#ifndef TJEK_API_PRIVATE_H
#define TJEK_API_PRIVATE_H

#include <time.h>

#define TJEK_API_BASE_URL "https://squid-api.tjek.com/v2/"
#define COOP365_DEALER_ID "DWZE1w"
#define DB_DEALER_ID "d311fg"

/**
 * Fetches id of active catalog from a given dealer using the Tjek API
 * @return freeable catalog id string or NULL if none is found
 */
char* get_active_catalog_id(char *dealer_id);

/**
 * Converts ISO8601 string to time_t object, assuming time string is UTC +0.
 */
time_t parse_iso8601_string(const char *time_str);

#endif