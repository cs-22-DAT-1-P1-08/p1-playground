#include <assert.h>
#include <stdio.h>
#include "../src/api/coop_api.h"
#include "../src/api/tjek_api.h"
#include "../src/api/location_api.h"
#include "../src/ui/main_view.h"
#include "../src/ui/shopping_list_view.h"
#include "../src/sorting_functions.h"
#include "../src/store.h"
#include <stdlib.h>

int main(void) {
    store_t *daglibrugsen = get_coop_store("1290", DB_DEALER_ID);
    store_t *coop365 = get_coop_store("24165", COOP365_DEALER_ID);
    store_t *stores[] = {daglibrugsen,
                         coop365};

    char *grocery_wares[] = {"Maelk", "Bread", "Smør", "Æg", "Hvidløg", "Løg", "Gulerødder", "Kartofler", "Tomater",
                              "Salat", "Bønner", "Linser", "Ris", "Kød", "Fisk", "Kylling", "Skink", "Leverpostej",
                              "Ost", "Yoghurt", "Flødeost", "Smoothie", "Saft", "Vand", "Coca Cola", "Øl", "Vin",
                              "Krydderier", "Bageingredienser", "Snacks", "Nødder", "Frugt", "Grøntsager", "Konserves",
                              "Dressing", "Marinade", "Sauce", "Ketchup", "Sennep", "Chokolade", "Kage", "Is", "Slik",
                              "Gummi slik", "Popcorn", "Pizza", "Lasagne", "Pasta", "Risotto", "Suppe", "Dessert",
                              "Smoothie bowl", "Juice", "Cocktail", "Kaffe", "Te", "Espresso", "Cappuccino", "Latte",
                              "Mokka", "Chai latte"};
    for (int i = 0; i < 60; ++i) {
        assert(compare_item(stores, grocery_wares[i], 2) != NULL);
    }
}