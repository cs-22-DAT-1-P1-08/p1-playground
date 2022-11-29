#ifndef TJEK_API_PRIVATE_H
#define TJEK_API_PRIVATE_H

#include <time.h>
#include <json-c/json.h>

#define TJEK_API_BASE_URL "https://squid-api.tjek.com/v2/"
#define TJEK_API_V4_BASE_URL "https://squid-api.tjek.com/v4/"

/**
 * Helper function: Loops through views (Tjek API, Incito) and finds offers
 */
void recursive_find_offers_in_views(json_object *j_view, catalog_offers_t *catalog_offers);


catalog_offers_t* get_catalog_products(catalog_info_t *catalog_info, catalog_offers_t *catalog_offers);

/**
 * Converts ISO8601 string to time_t object, assuming time string is UTC +0.
 */
time_t parse_iso8601_string(const char *time_str);

#endif