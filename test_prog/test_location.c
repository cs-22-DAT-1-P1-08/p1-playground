#include <assert.h>
#include <strings.h>
#include "../src/api/location_api.h"

#define COLOR_BOLD_RED "[1;31m"
#define COLOR_BOLD_GREEN "\033[1;32m"
#define COLOR_BOLD_WHITE "\033[1;97m"
#define COLOR_GREEN "\033[0;32m"

int main(void) {
    char *transport_mode = "car";
    char* here_api_key = getenv("HERE_API_KEY");
    if (here_api_key == NULL) {
        fprintf(stderr, "Failed to retrieve HERE_API_KEY from environment variables.");
        exit(EXIT_FAILURE);
    }

    location_t locations[4];
    for (int i = 0; i < 4; ++i) {
        initialize_location(&locations[i]);
    }
    set_address(&locations[0], HOUSE_NUMBER, "12");
    set_address(&locations[0], STREET, "Horsensvej");
    set_address(&locations[0], POSTAL_CODE, "8740");
    strcpy(locations[0].place_name, "home");

    strcpy(locations[1].place_name, "daglibrugsen");
    strcpy(locations[2].place_name, "coop365");

    locations[3] = locations[0];
    int *route_times = route_time(locations, transport_mode, here_api_key, 4);

    int x, y;

    if (locations[1].dest_on_route == 1) {
        x=1;
        y=2;
    }
    else {
        x=2;
        y=1;
    }

    printf(COLOR_BOLD_GREEN "The address %s \t %2s %-10s\n"
                                   "Closest %-15s is on %-17s %2s\t%-10s and is %d minuttes and %2d seconds away\n"
                                   "Closest %-15s is on %-17s %2s\t%-10s and is %d minuttes and %2d seconds away\n"
                                   "And %s again on %d minuttes and %d seconds \n"
                  COLOR_BOLD_GREEN "Time of entire trip was %d minuttes and %d seconds",
                            locations[0].street, locations[0].houseNumber, locations[0].city,
                            locations[x].place_name, locations[x].street, locations[x].houseNumber, locations[x].city,
                            route_times[1] / 60, route_times[1] % 60,
                            locations[y].place_name, locations[y].street, locations[y].houseNumber, locations[y].city,
                            route_times[2] / 60, route_times[2] % 60,
                            locations[3].place_name, route_times[3] / 60, route_times[3] % 60,
                            route_times[0] / 60, route_times[0] % 60
    );
    /*printf(COLOR_BOLD_GREEN "The address %s \t %2s %-10s\n"
                                   "Closest %-15s is on %-17s %2s\t%-10s and is %d m away\n"
                                   "Closest %-15s is on %-17s %2s\t%-10s and is %d m away\n"
                                   "And %s again is %d m away\n"
                  COLOR_BOLD_GREEN "Time of entire trip was %d m",
                            locations[0].street, locations[0].houseNumber, locations[0].city,
                            locations[x].place_name, locations[x].street, locations[x].houseNumber, locations[x].city,
                            route_times[1],
                            locations[y].place_name, locations[y].street, locations[y].houseNumber, locations[y].city,
                            route_times[2],
                            locations[3].place_name, route_times[3],
                            route_times[0]
    );*/
}