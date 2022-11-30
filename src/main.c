#include "api.h"
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>

int main() {
    char apikey[100];
    scanf("%s", apikey);
    geocode hjem;
    initialize_geocode(&hjem);
    strcpy(hjem.street, "Hadsundvej");
    strcpy(hjem.houseNumber, "44");
    strcpy(hjem.postalCode, "9000");
    strcpy(hjem.place_name, "home");
    addr_to_geo(&hjem, apikey);

    geocode spar;
    initialize_geocode(&spar);
    strcpy(spar.place_name, "rema");
    strcpy(spar.city, hjem.city);

    //store_to_geo(&spar, apikey, hjem.lat, hjem.lng);
    //for (int i = 0; i < 6; ++i) {
    //    printf("\n%s", get_address(&spar, i));
    //}
    geocode hjem_igen;
    initialize_geocode(&hjem_igen);
    strcpy(hjem_igen.street, "Hadsundvej");
    strcpy(hjem_igen.houseNumber, "44");
    strcpy(hjem_igen.postalCode, "9000");
    strcpy(hjem_igen.place_name, "home2");
    addr_to_geo(&hjem_igen, apikey);
    geocode places[] = {hjem, spar, hjem_igen};
    int *arr = route_time(places, "pedestrian", apikey, 3);

    for (int i = 0; i < 3; ++i) {
        printf("\n\n%d",arr[i]);

    }
    /*
    geocode hulla;
    initialize_geocode(&hulla);
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