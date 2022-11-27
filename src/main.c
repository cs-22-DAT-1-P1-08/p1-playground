#include <stdio.h>
#include <stdlib.h>
#include "tjek_api.h"
#include <curl/curl.h>
#include <locale.h>

void tjek_api_temp() {
    setlocale(LC_ALL, "");
    catalog_info_t *db_catalog_info = get_catalog_info(DB_DEALER_ID);
    printf("%s: %d\n", db_catalog_info->id, db_catalog_info->offer_count);

    get_catalog_offers(db_catalog_info);

    free_catalog_info(db_catalog_info);
}

int main() {
    tjek_api_temp();

    return 0;
}

