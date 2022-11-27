#include "tjek_api.h"
#include "tjek_api_p.h"
#include "util/curlext.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <time.h>

// WIP
void get_catalog_offers(catalog_info_t *catalog_info) {
    //for (int i = 0; i < 1; i++) {
    for (int i = 0; i < catalog_info->offer_count / 24 + 1; i++) {
        int offset = i * 24;
        char offset_str[16] = "";
        sprintf(offset_str, "%d", offset);

        char url[200] = TJEK_API_BASE_URL "offers?dealer_id=";
        strcat(url, catalog_info->dealer_id);
        strcat(url, "&catalog_id=");
        strcat(url, catalog_info->id);
        strcat(url, "&types=paged%2Cincito");
        strcat(url, "&offset=");
        strcat(url, offset_str);
        strcat(url, "&limit=24");

        char *response = curlext_easy_fetch(url, "https");
        
        json_object *j_root = json_tokener_parse(response);
        free(response);


        for (int j = 0; j < json_object_array_length(j_root); j++) {
            json_object *j_offer = json_object_array_get_idx(j_root, j);
            json_object *j_heading = json_object_object_get(j_offer, "heading");
            json_object *j_pricing = json_object_object_get(j_offer, "pricing");
            json_object *j_price = json_object_object_get(j_pricing, "price");

            printf("%.2lf kr: %s\n", json_object_get_double(j_price), json_object_get_string(j_heading));
        }
    }

}

catalog_info_t* get_catalog_info(char *dealer_id) {
    catalog_info_t *result = calloc(1, sizeof(catalog_info_t));
    result->dealer_id = dealer_id;

    /* Fetch data from API */
    char url[200] = TJEK_API_BASE_URL "dealerfront?limit=24&order_by=name&types=paged%2Cincito&offset=0&dealer_id=";
    strcat(url, dealer_id);

    // TODO: API & JSON error handling
    char *response = curlext_easy_fetch(url, "https");
    json_object *j_root = json_tokener_parse(response);
    free(response);

    /* Traverse JSON object to find catalog id */
    json_object *j_store = json_object_array_get_idx(j_root, 0);
    json_object *j_catalogs = json_object_object_get(j_store, "catalogs");

    // Iterate through catalogs to find first active catalog
    json_object *j_catalog = NULL;
    for (int i = 0; i < json_object_array_length(j_catalogs); i++) {
        j_catalog = json_object_array_get_idx(j_catalogs, i);

        json_object *j_start_time = json_object_object_get(j_catalog, "run_from");
        time_t start_time = parse_iso8601_string(json_object_get_string(j_start_time));

        json_object *j_end_time = json_object_object_get(j_catalog, "run_till");
        time_t end_time = parse_iso8601_string(json_object_get_string(j_end_time));

        // We found an active catalog, stop searching.
        time_t current_time = time(NULL);
        if (end_time > current_time && start_time < current_time) {
            result->start_time = start_time;
            result->end_time = end_time;
            break;
        }

        j_catalog = NULL;
    }

    // No active catalogs
    if (j_catalog == NULL)
        return result; // TODO: How should we handle errors here?

    json_object *j_offer_count = json_object_object_get(j_catalog, "offer_count");
    result->offer_count = json_object_get_int(j_offer_count);

    /* Convert catalog id JSON object to string */
    json_object *j_catalog_id = json_object_object_get(j_catalog, "incito_publication_id");
    size_t catalog_id_len = strlen(json_object_get_string(j_catalog_id));
    result->id = calloc(catalog_id_len + 1, sizeof(char));
    strcpy(result->id, json_object_get_string(j_catalog_id));

    return result;
}

void free_catalog_info(catalog_info_t *info) {
    free(info->id);
    free(info);
}

time_t parse_iso8601_string(const char *time_str) {
    struct tm cal_time;
    sscanf(time_str,"%d-%d-%dT%d:%d:%d",
           &cal_time.tm_year,
           &cal_time.tm_mon,
           &cal_time.tm_mday,
           &cal_time.tm_hour,
           &cal_time.tm_min,
           &cal_time.tm_sec
    );
    cal_time.tm_year -= 1900; /* years since 1900 */
    cal_time.tm_mon -= 1;

    // _mgkmtime handles timezones correctly, do not change to mktime
    return _mkgmtime(&cal_time);
}