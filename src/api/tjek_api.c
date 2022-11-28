#include "tjek_api.h"
#include "tjek_api_p.h"
#include "../util/curlext.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <curl/curl.h>
#include <time.h>



// WIP
catalog_offers_t* get_catalog_offers(catalog_info_t *catalog_info) {
    catalog_offers_t *result = calloc(1, sizeof(catalog_offers_t));
    result->offer_groups = calloc(1, sizeof(dlist_t));
    result->offers = calloc(1, sizeof(dlist_t));

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
            json_object *j_id = json_object_object_get(j_offer, "catalog_view_id");
            json_object *j_heading = json_object_object_get(j_offer, "heading");
            json_object *j_pricing = json_object_object_get(j_offer, "pricing");
            json_object *j_price = json_object_object_get(j_pricing, "price");

            int group_id_len = json_object_get_string_len(j_id);
            int heading_len = json_object_get_string_len(j_heading);
            if (group_id_len == 0 || heading_len == 0)
                continue;

            offer_group_t *group = calloc(1, sizeof(offer_group_t));

            group->id = calloc(group_id_len + 1, sizeof(char));
            strcpy(group->id, json_object_get_string(j_id));

            group->heading = calloc(heading_len + 1, sizeof(char));
            strcpy(group->heading, json_object_get_string(j_heading));

            group->price = json_object_get_double(j_price);
            group->offers = calloc(1, sizeof(dlist_t));
            dlist_add(result->offer_groups, group);
        }
    }

    return get_catalog_products(catalog_info, result);
}

void rec_child_views(json_object *j_view, catalog_offers_t *catalog_offers) {
    json_object *j_child_views = json_object_object_get(j_view, "child_views");

    if (json_object_get_type(j_child_views) != json_type_array)
        return;

    for (int i = 0; i < json_object_array_length(j_child_views); i++) {
        json_object *j_child_view = json_object_array_get_idx(j_child_views, i);
        rec_child_views(j_child_view, catalog_offers);

        json_object *j_child_role = json_object_object_get(j_child_view, "role");
        if (json_object_get_type(j_child_role) != json_type_string || strcmp(json_object_get_string(j_child_role), "offer") != 0)
            continue;

        json_object *j_offer = json_object_object_get(
                json_object_object_get(j_child_view, "meta"),
                "tjek.offer.v1"
        );

        json_object *j_products = json_object_object_get(j_offer, "products");
        for (int j = 0; j < json_object_array_length(j_products); j++) {
            json_object *j_product = json_object_array_get_idx(j_products, j);
            json_object *j_id = json_object_object_get(j_product, "id");
            json_object *j_title = json_object_object_get(j_product, "title");

            offer_t *offer = calloc(1, sizeof(offer_t));

            offer->ean = calloc(json_object_get_string_len(j_id) + 1, sizeof(char));
            strcpy(offer->ean, json_object_get_string(j_id));

            offer->title = calloc(json_object_get_string_len(j_title) + 1, sizeof(char));
            strcpy(offer->title, json_object_get_string(j_title));

            json_object *j_group_id = json_object_object_get(j_child_view, "id");
            char *group_id = json_object_get_string(j_group_id);

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

            dlist_add(catalog_offers->offers, offer);
        }
    }
}

// WIP
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
    json_object *j_root_view_children = json_object_object_get(j_root_view, "child_views");

    rec_child_views(j_root_view, catalog_offers);

    return catalog_offers;
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

void free_offer(offer_t *offer) {
    if (offer == NULL)
        return;

    free(offer->ean);
    free(offer->title);
    if (offer->group != NULL)
        free_offer_group(offer->group);
    free(offer);
}

void free_offer_group(offer_group_t *offer_group) {
    if (offer_group == NULL)
        return;

    dlist_free(offer_group->offers);
    free(offer_group->id);
    free(offer_group->heading);
    free(offer_group);
}

// i am too tired to do this properly
// TODO: maybe avoid exit code 0xC0000005 (Access violation error)
void free_catalog_offers(catalog_offers_t *catalog_offers) {
    if (catalog_offers == NULL)
        return;

    dlist_t *offer_groups = catalog_offers->offer_groups;
    if (offer_groups != NULL) {
        dlist_node_t *group_node = offer_groups->head;
        while (group_node != NULL) {
            if (group_node->data == NULL)
                continue;

            dlist_node_t *next = group_node->next;
            free_offer_group((offer_group_t*)group_node->data);
            group_node = next;
        }
        dlist_free(offer_groups);
    }

    dlist_t *offers = catalog_offers->offers;
    if (offers != NULL) {
        dlist_node_t *offer_node = offers->head;
        while (offer_node != NULL) {
            if (offer_node->data == NULL)
                continue;

            dlist_node_t *next = offer_node->next;
            free_offer((offer_t*)offer_node->data);
            offer_node = next;
        }
        dlist_free(offers);
    }

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