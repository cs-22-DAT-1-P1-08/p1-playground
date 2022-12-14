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

    char *grocery_case[] = {"kylling bryst", "1 KG", "guleroedder", "4 KG", "havregryn", "500 G",
                            "bananer", "8 STK","blaabaerd", "500 G", "sodavand", "4 L",
                            "revet ost", "1 KG","ost", "300 G", "tortillas", "8 STK",
                            "slik", "1 KG", "agurker", "2 STK", "pasta", "1 KG",
                           "syltetoej", "300 G", "kartofler", "3 KG","smoer", "600 G",
                           "gris", "1 KG","hakket oksekoed", "800 G", "aeg", "20 STK",
                           "skyr", "1 KG", "leverpostej", "500 G","broed", "1 KG",
                           "chokolade bar", "2 STK", "chips", "3 KG", "juice", "2 L",
                           "musli", "350 G", "aebler", "1 KG", "floede is", "500 G",
                           "maelk", "4 L","fiske fillet", "800 G"};

    /*char *grocery_case[] = {"kylling bryst", "guleroedder" , "havregryn", "bananer",
                            "blaabaer", "sodavand", "revet ost", "ost",
                            "tortillas", "blandet slik", "agurker", "pasta",
                            "syltetoej", "kartofler",  "smoer", "svin",
                            "hakket oksekoed", "aeg", "skyr", "leverpostej",
                            "broed", "chokolade bar", "chips", "juice",
                            "musli", "aebler", "floede is", "maelk",
                            "fiske fillet"};*/
    int bool_fail = 0;

    for (int i = 0; i < 58; i += 2) {
        amount_t *amount = find_amount_from_string(grocery_case[i+1]);

        char *temp_grocery = grocery_case[i];
        item_t *temp_item = compare_item(stores, temp_grocery, 2);
        if (temp_item == NULL) {
            printf("\033[1;31m%s failed\n", temp_grocery);
            bool_fail = 1;
        }
        else if (amount != NULL && temp_item->amount != NULL)
            printf("\033[1;32m%s passed as %d * %.2lf %s: %s\n", temp_grocery,
                   cmp_amount(*temp_item, *amount), temp_item->amount->amount, get_unit_name(temp_item->amount->unit_type), temp_item->name);

        else printf("\033[1;32m%s passed as %s\n", temp_grocery, temp_item->name);
    }
    free_store(daglibrugsen);
    free_store(coop365);
    return bool_fail;
}