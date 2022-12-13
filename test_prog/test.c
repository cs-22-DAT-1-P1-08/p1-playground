#include <assert.h>
#include "../src/api/coop_api.h"
#include "../src/sorting_functions.h"


int main(void) {
    store_t *daglibrugsen = get_coop_store("Dagli'Brugsen", "1290", DB_DEALER_ID);
    store_t *coop365 = get_coop_store("Coop 365", "24165", COOP365_DEALER_ID);
    store_t *stores[] = {daglibrugsen,
                         coop365};
    char *grocery_wares[] = {"MAELK", "BREAD", "SMOER", "AEG", "HVIDLOEG", "LOEG", "GULEROEDDER",
                             "KARTOFLER", "TOMATER", "SALAT", "BOENNER", "LINSER", "RIS",
                             "KOED", "FISK", "KYLLING", "SKINK", "LEVERPOSTEJ", "OST",
                             "YOGHURT", "FLOEDE OST" "SMOOTHIE", "SAFT", "VAND", "COCA COLA",
                             "OEL", "VIN", "KRYDDERIER", "BAGEINGREDIENSER", "SNACKS",
                             "NOEDDER", "FRUGT", "GROENTSAGER", "KONSERVER", "DRESSING",
                             "MARINADE", "SAUCE", "KETCHUP", "SENNEP", "CHOKOLADE",
                             "KAGE", "IS", "SLIK", "GUMMI SLIK", "POPCORN", "PIZZA",
                             "LASAGNE", "PASTA", "RISOTTO", "SUPPE", "DESSERT",
                             "SMOOTHIE BOWL", "JUICE", "COCKTAIL", "KAFFE", "TE", "ESPRESSO",
                             "CAPPUCCINO", "LATTE", "MOKKA", "CHAI LATTE"};

    int bool_fail = 0;
    for (int i = 0; i < 60; ++i) {
        item_t *temp_item = compare_item(stores, grocery_wares[i], 2);
        //item_t *temp_item = find_cheapest_match(coop365, grocery_wares[i]);
        if (temp_item == NULL) {
            printf("\033[1;31m%s failed\n", grocery_wares[i]);
            bool_fail = 1;
        } else printf("\033[1;32m%s passed as %s\n", grocery_wares[i], temp_item->name);
    }
    free_store(daglibrugsen);
    free_store(coop365);
    return bool_fail;
}