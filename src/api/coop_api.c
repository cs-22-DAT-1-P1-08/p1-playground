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

// Proxy with api from above with certain changes, such as æ, ø and å made into ae, oe and aa.
// Also only updates every 12 hours to take into account the 200 pulls a week limit for the API
#define COOP_PRODUCT_API_URL "https://p1.theodor.dev/coop-api-proxy/v1/product/"

dlist_t* coop_get_items(char* store_id){
    /* Merges the URL with the store ID to retrieve data from relevant API */
    char full_url[100];
    strcpy(full_url, COOP_PRODUCT_API_URL);
    strcat(full_url, store_id);

    /* Retrieve URL contents as text */
    char* res = curlext_easy_fetch(full_url, "https");

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

        item->details = calloc(strlen(json_object_get_string(j_amount)) + 1, sizeof(char));
        strcpy(item->details, json_object_get_string(j_amount));

        item->amount = find_amount_from_string(item->details);
        if (item->amount == NULL)
            item->amount = find_amount_from_string(item->name);

        item->price = json_object_get_double(j_price);

        dlist_add(item_list, item);
    }
    return item_list;
}

/* Deallocate memory assigned to item */
void free_item(item_t* item){
    if (item == NULL)
        return;

    if (item->name != NULL)
        free(item->name);

    if (item->ean != NULL)
        free(item->ean);

    if (item->details != NULL)
        free(item->details);

    if (item->amount != NULL)
        free(item->amount);

    free(item);
}

amount_t* find_amount_from_string(char* input_str){
    static const int NUMBER_BUFFER_SIZE = 20;

    amount_t *result = calloc(1, sizeof(amount_t));
    int input_len = (int)strlen(input_str);
    result->amount = -1;

    double multiplier = 1;

    char number_buffer[NUMBER_BUFFER_SIZE];
    int number_len = 0;

    for (int i = 0; i < input_len; i++) {
        // Ignore whitespace
        if (input_str[i] == ' ') continue;

        /* Handle number sequences */
        if (strchr("0123456789", input_str[i])) {
            // reset existing number buffer
            number_len = 0;
            memset(number_buffer, 0, sizeof(number_buffer));

            while (i < input_len && strchr("0123456789", input_str[i])) {
                number_buffer[number_len++] = input_str[i];
                i++;
            }

            continue;
        }

        /* Handle character sequences */
        int start_index = i;

        while (++i < input_len && isalpha(input_str[i])) continue;
        int len = i - start_index;

        // Did not find sequence or does not have an amount number stored
        if (len == 0 && number_len == 0) continue;

        // Get this character sequence as separate string (substring)
        char word[len + 1];
        word[len] = '\0';

        memcpy(word, &input_str[start_index], len);

        /* Handle unit or multiplier keywords */
        if (strcmp(word, "G") == 0 || strcmp(word, "GRAM") == 0) {
            result->unit_type = GRAMS;
            result->amount = multiplier * atoi(number_buffer);
        }
        else if (strcmp(word, "KG") == 0) {
            result->unit_type = GRAMS;
            result->amount = multiplier * atoi(number_buffer) * 1000;
        }
        else if (strcmp(word, "L") == 0 || strcmp(word, "LITER") == 0){
            result->unit_type = LITERS;
            result->amount = multiplier * atoi(number_buffer);
        }
        else if (strcmp(word, "CL") == 0 || strcmp(word, "CENTILITER") == 0){
            result->unit_type = LITERS;
            result->amount = multiplier * atoi(number_buffer)/100;
        }
        else if (strcmp(word, "DL") == 0 || strcmp(word, "DECILITER") == 0){
            result->unit_type = LITERS;
            result->amount = multiplier * atoi(number_buffer)/10;
        }
        else if (strcmp(word, "ML") == 0 || strcmp(word, "MILLILITER") == 0){
            result->unit_type = LITERS;
            result->amount = multiplier * atoi(number_buffer)/100;
        }
        else if (strcmp(word, "STK") == 0){
            result->unit_type = PIECES;
            result->amount = multiplier * atoi(number_buffer);
        }
        else if (strcmp(word, "X") == 0) {
            multiplier = (double)atoi(number_buffer);
        }
    }

    // Amount not found in string
    if (result->amount == -1) {
        free(result);
        return NULL;
    }

    return result;
}

const char* get_unit_name(enum AmountUnit unit) {
    static const char* names[] = { "grams", "liters", "pieces"};
    return names[unit];
}
