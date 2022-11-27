#include "tjek_api_p.h"
#include "util/curlext.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <time.h>

// TODO: Remove temp main function
int main() {
    char *catalog_id = get_active_catalog_id(COOP365_DEALER_ID);
    printf("%s\n\n", catalog_id);
    free(catalog_id);
    return 0;
}

char* get_active_catalog_id(char *dealer_id) {
    /* Fetch data from API */
    char url[200] = TJEK_API_BASE_URL "dealerfront?limit=24&order_by=name&types=paged%2Cincito&offset=0&dealer_id=";
    strcat(url, dealer_id);

    // TODO: API & JSON error handling
    char *response = curlext_easy_fetch(url, "https");

    /* Create JSON object */
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
        if (end_time > time(NULL) && start_time < time(NULL))
            break;

        j_catalog = NULL;
    }

    // No active catalogs
    if (j_catalog == NULL)
        return NULL;

    json_object *j_catalog_id = json_object_object_get(j_catalog, "incito_publication_id");

    /* Convert catalog id JSON object to string */
    size_t catalog_id_len = strlen(json_object_get_string(j_catalog_id));
    char *catalog_id = calloc(catalog_id_len + 1, sizeof(char));
    strcpy(catalog_id, json_object_get_string(j_catalog_id));

    return catalog_id;
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

    // _mgkmtime converts properly from UTC +0 to local time
    return _mkgmtime(&cal_time);
}