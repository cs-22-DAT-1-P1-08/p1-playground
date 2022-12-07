#include "api.h"
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>

int main() {
    char apikey[100];
    scanf("%s", apikey);
    location_t hjem;
    initialize_location(&hjem);
    strcpy(hjem.street, "Hadsundvej");
    strcpy(hjem.houseNumber, "44");
    strcpy(hjem.postalCode, "9000");
    strcpy(hjem.place_name, "home");
    addr_to_geo(&hjem, apikey);

    location_t spar;
    initialize_location(&spar);
    strcpy(spar.place_name, "rema");
    strcpy(spar.city, hjem.city);

    //store_to_geo(&spar, apikey, hjem.lat, hjem.lng);
    //for (int i = 0; i < 6; ++i) {
    //    printf("\n%s", get_address(&spar, i));
    //}
    location_t places[] = {hjem, spar, hjem};
    int *arr = route_time(places, "bicycle", apikey, 3);

    printf("entire route duration: %d", arr[0]);
    for (int i = 1; i < 3; ++i) {
        char temp_dest_name[40];
        for (int j = 0; j < 3; ++j) {
            if (places[j].dest_on_route == i) {
                strcpy(temp_dest_name, places[j].place_name);
                break;
            }
        }
        printf("\n\n%s %d",temp_dest_name, arr[i]);
    }
    /*
    location_t hulla;
    initialize_location(&hulla);
    strcpy(hulla.street, "Hadsundvej");
    strcpy(hulla.houseNumber, "44");
    char apikey[100];
    scanf("%s", apikey);
    addr_to_geo(&hulla,apikey);
    printf("street:%s county:%s houseNumber: %s postalCode: %s state: %s city: %s lat: %s lng: %s",hulla.street, hulla.county, hulla.houseNumber, hulla.postalCode, hulla.state, hulla.city, hulla.lat, hulla.lng);
    */


    /*char url[200] = "https://discover.search.hereapi.com/v1/discover?at=57.04074,9.95146&limit=1&q=fakta&apiKey=";
    char apikey[100];
    scanf("%s", apikey);
    add_strings(url, 1, apikey);
    char *str = malloc(sizeof(char) * 100000);
    if (str == NULL) {
        perror("could not create big string");
        exit(1);
    }
    api_to_str(url, "https", str);

    printf("%s\n\n", str);
    free(str);*/
    return 0;
}