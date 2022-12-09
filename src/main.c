#include <stdio.h>
#include "api/coop_api.h"
#include "api/tjek_api.h"
#include "api/location_api.h
#include "ui/main_view.h"
#include "sorting_functions.h"
#include "store.h"
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <locale.h>
#include <wchar.h>
#include <curses.h>

int main() {
    initscr();
    noecho();

    WINDOW *main_view = create_main_view(stdscr);
    render_main_view(main_view);
    destroy_main_view(main_view);



    getch();
    endwin();
    return 0;
    
    printf("Started program...\n");

    // Retrieve HERE API key
    char* here_api_key = getenv("HERE_API_KEY");
    if (here_api_key == NULL) {
        fprintf(stderr, "Failed to retrieve HERE_API_KEY from environment variables.");
        exit(EXIT_FAILURE);
    }

    // Fill home location
    geocode home;

    initialize_geocode(&home);
    strcpy(home.street, "Hadsundvej");
    strcpy(home.houseNumber, "44");
    strcpy(home.postalCode, "9000");
    strcpy(home.place_name, "home");
    addr_to_geo(&home, here_api_key);

    // Prepare stores
    store_t *daglibrugsen = get_coop_store("Dagli'Brugsen", "1290", DB_DEALER_ID);
    store_t *coop365 = get_coop_store("Coop 365", "24165", COOP365_DEALER_ID);

    // Fill store locations
    fill_nearest_store(daglibrugsen, &home);
    fill_nearest_store(coop365, &home);

    // Get the route time
    /*geocode places[] = {home, *coop365->location, *daglibrugsen->location};
    int *arr = route_time(places, "car", here_api_key, 3);

    for (int i = 0; i < 3; i++) {
        printf("\n\n%d", arr[i]);
    }*/

    // Grænseflade implementation
    printf("%s: %lf", find_cheapest_match(daglibrugsen, "TUBORG").name,
           find_cheapest_match(daglibrugsen, "TUBORG").price);

    free_store(daglibrugsen);
    free_store(coop365);
    return 0;
}