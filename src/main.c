#include <stdio.h>
#include "store.h"
#include "api/tjek_api.h"
#include "sorting_functions.h"

int main() {
    printf("Started program...\n");
    store_t *daglibrugsen = get_coop_store("1290", DB_DEALER_ID);
    store_t *coop365 = get_coop_store("24165", COOP365_DEALER_ID);
    store_t *stores[] = {daglibrugsen,
                        coop365};
    //char *search_term[] = {"AGURK", "TOMAT"};
    // Grænseflade implementation
    char *search_term = "AGURK";


    item_t *cheapest_store_item  = find_cheapest_match(stores[0], search_term);
    printf("%s: %lf\n", cheapest_store_item->name, cheapest_store_item->price);
    //printf("%s: %lf", cheapest_store_item->name, cheapest_store_item->price);
    for (int i = 0; i < 2; ++i) {
        print_items(stores[i], search_term);
    }
    //

    //item_t *cheapest_overall_item = compare_item(stores, search_term, 2);
    //printf("%s: %lf", cheapest_overall_item->name, cheapest_overall_item->price);

    //printf("COOP365 %s: %lf", find_cheapest_match(coop365, "AGURK").name,find_cheapest_match(coop365, "AGURK").price);
    free_store(daglibrugsen);
    free_store(coop365);
    return 0;
}
