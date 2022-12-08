#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <json-c/json.h>
#include <curl/curl.h>
#include "tjek_api.h"
#include "tjek_api_p.h"
#include "../util/curlext.h"
#include "../common.h"

catalog_info_t* get_catalog_info(char *dealer_id) {
    catalog_info_t *result = calloc(1, sizeof(catalog_info_t));
    result->dealer_id = dealer_id;

    /* Fetch data from API */
    char url[200] = TJEK_API_BASE_URL "dealerfront?limit=24&order_by=name&types=paged%2Cincito&offset=0&dealer_id=";
    strcat(url, dealer_id);

    char *response = curlext_easy_fetch(url, "https");
    json_object *j_root = json_tokener_parse(response);

    // Handle invalid JSON data
    if (j_root == NULL ||
        json_object_get_type(j_root) != json_type_array ||
        json_object_array_length(j_root) == 0) {

        fprintf(stderr,
                "Received invalid data from tjek_api (in get_catalog_info): \n%s",
                (response == NULL) ? "(null)" : response
        );

        exit(EXIT_FAILURE);
    }

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
    if (j_catalog == NULL) {
        free_catalog_info(result);
        return NULL;
    }

    json_object *j_offer_count = json_object_object_get(j_catalog, "offer_count");
    result->offer_count = json_object_get_int(j_offer_count);

    /* Convert catalog id JSON object to string */
    json_object *j_catalog_id = json_object_object_get(j_catalog, "incito_publication_id");
    size_t catalog_id_len = strlen(json_object_get_string(j_catalog_id));
    result->id = calloc(catalog_id_len + 1, sizeof(char));
    strcpy(result->id, json_object_get_string(j_catalog_id));

    return result;
}

catalog_offers_t* get_catalog_offers(catalog_info_t *catalog_info) {
    catalog_offers_t *result = calloc(1, sizeof(catalog_offers_t));
    result->offer_groups = calloc(1, sizeof(dlist_t));
    result->products = calloc(1, sizeof(dlist_t));

    for (int i = 0; i < catalog_info->offer_count / 24 + 1; i++) {
        int offset = i * 24;
        char offset_str[16] = "";
        sprintf(offset_str, "%d", offset);

        char url[200] = TJEK_API_BASE_URL "offers?types=paged%2Cincito&dealer_id=";
        strcat(url, catalog_info->dealer_id);
        strcat(url, "&catalog_id=");
        strcat(url, catalog_info->id);
        strcat(url, "&offset=");
        strcat(url, offset_str);
        strcat(url, "&limit=24");

        char *response = curlext_easy_fetch(url, "https");

        json_object *j_root = json_tokener_parse(response);
        free(response);


        for (int j = 0; j < json_object_array_length(j_root); j++) {
            json_object *j_offer = json_object_array_get_idx(j_root, j);
            json_object *j_id = json_object_object_get(j_offer, "catalog_view_id");
            json_object *j_heading = json_object_object_get(j_offer, "heading");
            json_object *j_pricing = json_object_object_get(j_offer, "pricing");
            json_object *j_price = json_object_object_get(j_pricing, "price");

            int group_id_len = json_object_get_string_len(j_id);
            int heading_len = json_object_get_string_len(j_heading);
            if (group_id_len == 0 || heading_len == 0) {
                debug_print("Received invalid offer group, skipping...\n");
                continue;
            }

            offer_group_t *group = calloc(1, sizeof(offer_group_t));

            group->id = calloc(group_id_len + 1, sizeof(char));
            strcpy(group->id, json_object_get_string(j_id));

            group->heading = calloc(heading_len + 1, sizeof(char));
            strcpy(group->heading, json_object_get_string(j_heading));

            group->price = json_object_get_double(j_price);
            group->offers = calloc(1, sizeof(dlist_t));
            dlist_add(result->offer_groups, group);

            debug_print("Parsed offer group with id: %s\n", group->id);
        }
    }

    return get_catalog_products(catalog_info, result);
}

catalog_offers_t* get_catalog_products(catalog_info_t *catalog_info, catalog_offers_t *catalog_offers) {
    char* url = TJEK_API_V4_BASE_URL "rpc/generate_incito_from_publication";

    char request_body[500] = "{\n"
                     "    \"device_category\": \"desktop\",\n"
                     "    \"pointer\": \"fine\",\n"
                     "    \"orientation\": \"horizontal\",\n"
                     "    \"pixel_ratio\": 1.25,\n"
                     "    \"max_width\": 818,\n"
                     "    \"versions_supported\": [\n"
                     "        \"1.0.0\"\n"
                     "    ],\n"
                     "    \"locale_code\": \"en-US\",\n"
                     "    \"feature_labels\": [\n"
                     "        {\n"
                     "            \"key\": \"x\",\n"
                     "            \"value\": 1\n"
                     "        }\n"
                     "    ],\n"
                     "    \"id\": \"";
    strcat(request_body, catalog_info->id);
    strcat(request_body, "\"\n}");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "X-Api-Key: none");
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    char *response = curlext_easy_pfetch(url, "HTTPS", request_body, headers);
    curl_slist_free_all(headers);

    json_object *j_root = json_tokener_parse(response);
    free(response);

    json_object *j_root_view = json_object_object_get(j_root, "root_view");

    recursive_find_offers_in_views(j_root_view, catalog_offers);

    return catalog_offers;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
void recursive_find_offers_in_views(json_object *j_view, catalog_offers_t *catalog_offers) {
    json_object *j_child_views = json_object_object_get(j_view, "child_views");

    if (json_object_get_type(j_child_views) != json_type_array)
        return;

    /* Loop through view children */
    for (int i = 0; i < json_object_array_length(j_child_views); i++) {
        json_object *j_child_view = json_object_array_get_idx(j_child_views, i);
        recursive_find_offers_in_views(j_child_view, catalog_offers);

        /* Collect data from views with role "offer" */
        json_object *j_child_role = json_object_object_get(j_child_view, "role");
        if (json_object_get_type(j_child_role) != json_type_string ||
            strcmp(json_object_get_string(j_child_role), "offer") != 0)
            continue;

        json_object *j_offer = json_object_object_get(
                json_object_object_get(j_child_view, "meta"),
                "tjek.offer.v1"
        );

        /* Loop through and parse products */
        json_object *j_products = json_object_object_get(j_offer, "products");
        for (int j = 0; j < json_object_array_length(j_products); j++) {
            json_object *j_product = json_object_array_get_idx(j_products, j);
            json_object *j_id = json_object_object_get(j_product, "id");
            json_object *j_title = json_object_object_get(j_product, "title");

            int ean_len = json_object_get_string_len(j_id);
            int title_len = json_object_get_string_len(j_title);

            // Skip if data is invalid
            if (ean_len == 0 || title_len == 0)
                continue;

            offer_t *offer = calloc(1, sizeof(offer_t));

            offer->ean = calloc(ean_len + 1, sizeof(char));
            strcpy(offer->ean, json_object_get_string(j_id));

            offer->title = calloc( title_len + 1, sizeof(char));
            strcpy(offer->title, json_object_get_string(j_title));

            json_object *j_group_id = json_object_object_get(j_child_view, "id");
            const char *group_id = json_object_get_string(j_group_id);

            /* Match offer to offer group */
            dlist_node_t *group_node = catalog_offers->offer_groups->head;
            while (group_node != NULL) {
                offer_group_t *group = (offer_group_t*)group_node->data;

                if (group->id == NULL || strcmp(group->id, group_id) != 0) {
                    group_node = group_node->next;
                    continue;
                }

                // found matching offer group, stop searching
                offer->group = group;
                dlist_add(group->offers, offer);
                break;
            }

            // Could not find offer group
            if (offer->group == NULL) {
                debug_print("Could not find matching offer group, freeing offer: %s %s %s\n",
                            offer->ean, offer->title, group_id);
                free(offer);
                continue;
            }

            dlist_add(catalog_offers->products, offer);
        }
    }
}
#pragma clang diagnostic pop

void free_catalog_info(catalog_info_t *info) {
    if (info == NULL)
        return;

    if (info->id != NULL)
        free(info->id);

    free(info);
}

void free_offer(offer_t *offer) {
    if (offer == NULL)
        return;

    if (offer->ean != NULL)
        free(offer->ean);

    if (offer->title != NULL)
        free(offer->title);

    free(offer);
}

void free_offer_group(offer_group_t *offer_group) {
    if (offer_group == NULL)
        return;

    dlist_free(offer_group->offers);

    if (offer_group->id != NULL)
        free(offer_group->id);

    if (offer_group->heading != NULL)
        free(offer_group->heading);

    free(offer_group);
}

void free_catalog_offers(catalog_offers_t *catalog_offers) {
    if (catalog_offers == NULL)
        return;

    dlist_free_all(catalog_offers->offer_groups, (void (*)(void *)) free_offer_group);
    dlist_free_all(catalog_offers->products, (void (*)(void *)) free_offer);
    free(catalog_offers);
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