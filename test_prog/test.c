#include <assert.h>
#include <strings.h>
#include "../src/api/coop_api.h"
#include "../src/sorting_functions.h"

#define COLOR_BOLD_RED "[1;31m"
#define COLOR_BOLD_GREEN "\033[1;32m"
#define COLOR_BOLD_WHITE "\033[1;97m"
#define COLOR_GREEN "\033[0;32m"

store_t* get_store_time_test(char* store_name, char* store_id, char* dealer_id);

int main(void) {

    clock_t begin = clock();
    store_t *daglibrugsen = get_store_time_test("Dagli'Brugsen", "1290", DB_DEALER_ID);
    store_t *coop365 = get_store_time_test("Coop 365", "24165", COOP365_DEALER_ID);
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf(COLOR_BOLD_WHITE "Initialized program in %lf seconds\n\n", time_spent);

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

    char *grocery_case[] = {"kyllingebryst", "1 KG", "guleroedder", "4 KG", "havregryn", "500 G",
                            "bananer", "8 STK","blaabaer", "500 G", "sodavand", "4 L",
                            "revet ost", "1 KG","ost", "300 G", "tortilla", "8 STK",
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
    printf("Testing example case:\n");
    int bool_fail = 0;

    double sum = 0;
    for (int i = 0; i < 58; i += 2) {
        amount_t *amount = find_amount_from_string(grocery_case[i+1]);

        char *temp_grocery = grocery_case[i];
        item_t *temp_item = compare_item(stores, temp_grocery, 2);
        if (temp_item == NULL) {
            printf(COLOR_BOLD_RED "%s failed\n", temp_grocery);
            bool_fail = 1;
        }
        else if (amount != NULL && temp_item->amount != NULL) {
            int product_amount = cmp_amount(*temp_item, *amount);
            printf(COLOR_BOLD_GREEN "%-15s passed as %2d * %7.2lf %-6s: %-30s \t costing %2d * %.2lf,-\n",
                   temp_grocery, product_amount,
                   temp_item->amount->amount,
                   get_unit_name(temp_item->amount->unit_type),
                   temp_item->name,
                   product_amount,
                   get_item_price(temp_item));
            sum += get_item_price(temp_item) * product_amount;
        }
        else {
            printf(COLOR_BOLD_GREEN "%-15s passed as                    : %-30s \t costing      %.2lf,-\n",
                   temp_grocery,
                   temp_item->name,
                   get_item_price(temp_item));
            sum += get_item_price(temp_item);
        }
    }
    printf("\033[1;32m------------------------------------------------------------------------------------------------------\n");
    printf("\033[1;32m Sum of products                                                              \t costing      %7.2lf,-", sum);

    free_store(daglibrugsen);
    free_store(coop365);
    return bool_fail;
}

store_t* get_store_time_test(char* store_name, char* store_id, char* dealer_id) {
    printf("Initializing store: %s\n", store_name);
    store_t *store = calloc(1, sizeof(store_t));

    clock_t begin, end;
    double time_spent;

    /* Fill store name */
    store->name = calloc(strlen(store_name) + 1, sizeof(char));
    strcpy(store->name, store_name);

    /* Fill assortment data from Coop API */
    begin = clock();
    store->products = coop_get_items("1290");
    end = clock();

    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf(COLOR_GREEN " - Fetched data from Coop API in %lf seconds\n", time_spent);

    /* Fill offer data from Tjek API */
    begin = clock();
    store->catalog_info = get_catalog_info(DB_DEALER_ID);
    store->offers = get_catalog_offers(store->catalog_info);
    end = clock();

    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf(COLOR_GREEN " - Fetched data from Tjek API in %lf seconds\n", time_spent);

    /* Link product and offer data */
    begin = clock();
    link_offer_data(store);
    end = clock();

    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf(COLOR_GREEN " - Linked %zu products and %zu offers in %lf seconds\n", store->products->count, store->offers->products->count, time_spent);

    return store;
}