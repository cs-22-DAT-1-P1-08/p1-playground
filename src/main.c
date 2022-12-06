#include "api.h"
#include <stdlib.h>
#include <stdio.h>
#include "store.h"
#include "api/tjek_api.h"
#include "sorting_functions.h"
#include <curl/curl.h>
#include <json-c/json.h>

int main() {
    printf("Started program...\n");
    store_t *daglibrugsen = get_coop_store("1290", DB_DEALER_ID);
    store_t *coop365 = get_coop_store("24165", COOP365_DEALER_ID);

    // Grænseflade implementation
    printf("%s: %lf", find_cheapest_match(daglibrugsen, "TUBORG").name,find_cheapest_match(daglibrugsen, "TUBORG").price);

    free_store(daglibrugsen);
    free_store(coop365);
    return 0;
}