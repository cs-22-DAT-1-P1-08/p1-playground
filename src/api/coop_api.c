//
// Created by carl on 28/11/2022.
//
#include "coop_api.h"
#include "../util/curlext.h"
#include "../util/dlist.h"
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include "curl/curl.h"



#define COOP_PRODUCT_API_URL "https://api.cl.coop.dk/productapi/v1/product/"

// Proxy with api from above with certain changes, such as æ, ø and å made into ae, oe and aa.
// Also only updates every 12 hours to take into account the 200 pulls a week limit for the API
#define GROUP_API_PROXY "https://p1.theodor.dev/coop-api-proxy/v1/product/"




dlist_t* coop_get_items(char* store_id){
    /* Merges the URL with the store ID to retrieve data from relevant API */
    char full_url[100];
    strcpy(full_url, GROUP_API_PROXY);
    strcat(full_url, store_id);

    /* Creates API key from environment variables and the prefix needed */
    char full_key[100] = "Ocp-Apim-Subscription-Key: ";
    char* key = getenv("CoopAPIKey");
    if (key == NULL){
        fprintf(stderr, "Failed to find CoopAPIKey in environment variables");
        exit(EXIT_FAILURE);
    }
    strcat(full_key, key);

    /* Retrieve URL contents as text */
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, full_key);
    char* res = curlext_easy_hfetch(full_url, "https", "", headers);
    curl_slist_free_all(headers);

    /* Convert retrieved text to json object */
    json_object* root = json_tokener_parse(res);
    if (root == NULL || json_object_get_type(root) != json_type_array){
        fprintf(stderr, "Received invalid data from COOP API\n%s", res);
        exit(EXIT_FAILURE);
    }
    free(res);

    /* Creates list from json object with data from API */
    dlist_t* item_list = calloc(1, sizeof(dlist_t));
    for (int i = 0; i < json_object_array_length(root); ++i) {
        item_t* item = calloc(1, sizeof(item_t));
        json_object* j_item = json_object_array_get_idx(root, i);
        json_object* j_name = json_object_object_get(j_item, "Navn");
        json_object* j_ean = json_object_object_get(j_item, "Ean");
        json_object* j_price = json_object_object_get(j_item, "Pris");
        json_object* j_amount = json_object_object_get(j_item, "Navn2");

        item->name = calloc(strlen(json_object_get_string(j_name)) + 1, sizeof(char));
        strcpy(item->name, json_object_get_string(j_name));

        item->ean = calloc(strlen(json_object_get_string(j_ean)) + 1, sizeof(char));
        strcpy(item->ean, json_object_get_string(j_ean));

        item->amount = calloc(strlen(json_object_get_string(j_amount)) + 1, sizeof(char));
        strcpy(item->amount, json_object_get_string(j_amount));

        item->price = json_object_get_double(j_price);

        dlist_add(item_list, item);
    }
    return item_list;
}

/* Deallocate memory assigned to item */
void free_item(item_t* item){
    free(item->name);
    free(item->ean);
    free(item->amount);
    free(item);
}

