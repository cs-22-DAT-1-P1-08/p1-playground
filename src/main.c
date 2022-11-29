#include "api.h"
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>

int main() {
  geocode hulla;
    initialize_geocode(&hulla);
    strcpy(hulla.street, "Hadsundvej");
    strcpy(hulla.houseNumber, "44");
    char apikey[100];
    scanf("%s", apikey);
    addr_to_geo(&hulla,apikey);
    printf("street:%s county:%s houseNumber: %s postalCode: %s state: %s city: %s lat: %s lng: %s",hulla.street, hulla.county, hulla.houseNumber, hulla.postalCode, hulla.state, hulla.city, hulla.lat, hulla.lng);

    printf(" \n\nhouseNumber: %s", hulla.houseNumber);
    return 0;
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

    return 0;*/
}