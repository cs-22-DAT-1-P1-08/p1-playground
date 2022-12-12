#include <stdio.h>
#include "store.h"
#include "api/tjek_api.h"
#include "sorting_functions.h"
#include <string.h>

int main() {
    printf("Started program...\n");
    store_t *daglibrugsen = get_coop_store("1290", DB_DEALER_ID);
    store_t *coop365 = get_coop_store("24165", COOP365_DEALER_ID);
    store_t *stores[] = {daglibrugsen,
                         coop365};
    // Grænseflade implementation
    char *search_term[] = {"SMOERBAR", "CHIP", "AGURK"};
    char *amount = "700 GRAMS ";


    //mads_is_a_PDF_FILE();


    //find_amount(amount);
    //Output
    print_item(stores, search_term, 2, 3);

    free_store(daglibrugsen);
    free_store(coop365);
    return 0;
}
