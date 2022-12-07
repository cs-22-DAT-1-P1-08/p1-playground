#include "api.h"
#include "api_p.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <stdarg.h>
#include <json-c/json.h>
#include "util/curlext.h"

void add_strings(char *str_dest, int args,...) {
    va_list valist;
    va_start(valist, args);
    for (int i = 0; i < args; ++i) {
        const char *str = (char *)va_arg( valist, const char* );
        strcat(str_dest, str);
    }
    va_end(valist);
}

void add_if_api(char *url_dest, char* apikey) {
    if (strcmp(apikey, "") != 0) {
        add_strings(url_dest, 3, "apiKey=", apikey, "&");
    }
}
void str_for_url(char* url, char* temp, int empty) {
    if (empty < 1) strcat(url, ",");
    strcat(url, "%20");
    strcat(url, temp);
}

void add_location_to_url(location *place, char* url, void f(char *, char *, int)) {
    char temp[100];
    int empty = 1;
    for (int i = 0; i < 6; ++i) {
        strcpy(temp, get_address(place, i));
        if (strcmp(temp, "") != 0) {
            f(url, temp, empty);
            empty = 0;
        }
    }
}

int is_addr_empty(location *place) {
    char temp[100];
    int empty = 1;
    for (int i = 0; i < 6; ++i) {
        strcpy(temp, get_address(place, i));
        if (strcmp(temp, "") != 0) {
            empty = 0;
        }
    }
    return empty;
}

void addr_to_geo(location *place, char* apikey) {
    char url[200] = "https://geocode.search.hereapi.com/v1/geocode?";
    add_if_api(url, apikey);
    strcat(url, "q=");
    add_location_to_url(place, url, &str_for_url);

    char *str = curlext_easy_fetch(url, "https");
    fill_location(place, str, 0);
    free(str);
}

void replace_address(location *place, json_object *address, int args,...) {
    va_list valist;
    va_start(valist, args);
    for (int i = 0; i < args; ++i) {
        const char *str = (char *)va_arg( valist, const char* );
        const char *source = json_object_get_string(json_object_object_get(address, str));
        set_address(place, i, (char *) source);
    }
    va_end(valist);
}

void fill_location(location *place, char *str, int item_num){
    json_object *jobj =json_tokener_parse(str);
    json_object *items = json_object_object_get(jobj, "items");
    json_object *item = json_object_array_get_idx(items, item_num);
    json_object *address = json_object_object_get(item, "address");
    replace_address(place, address,6, "street","houseNumber","city","postalCode","county","state");
    json_object *postion = json_object_object_get(item, "position");
    strcpy(place->lat, json_object_get_string(json_object_object_get(postion, "lat")));
    strcpy(place->lng, json_object_get_string(json_object_object_get(postion, "lng")));
}

//https://browse.search.hereapi.com/v1/browse?at=57.04074,9.95146&categories=600-6300-0066,800-8500-0178&name=rema 1000&apiKey=4nt5IVcSUaha7lK7Bx8f3PagaNfgP6QRyEYF3ZOMksA
void store_to_geo(location *store, char* apikey, char *lat, char *lng) {
    char url[200] = "https://browse.search.hereapi.com/v1/browse?";
    add_if_api(url, apikey);
    add_strings(url, 6, "at=", lat, ",", lng, "&categories=600-6300-0066&name=", store->place_name);
    char *str = curlext_easy_fetch(url, "https");
    int i;
    json_object *find_payment = NULL;
    json_object *items = json_tokener_parse(str);
    items = json_object_object_get(items, "items");
    for ( i = 0; i < json_object_array_length(items); ++i) {
        find_payment = json_object_array_get_idx(items, i);
        find_payment = json_object_object_get(find_payment, "payment");
        if (find_payment != NULL) break;
    }
    if (find_payment == NULL) printf("ERROR Store is not a store");
    fill_location(store, str, i);
    free(str);
}

int sum_of_arr(int arr[], size_t arr_len) {
    int sum = 0;
    for (int i = 1; i < arr_len; ++i) {
        sum += arr[i];
    }
    return sum;
}
int *json_to_traveltime(json_object *jobj, location* places, size_t places_len) {
    int *travel_time = malloc(sizeof(int) * places_len);

    // Searches through the json both through elements and the 1st and only route in routes
    json_object *routes = json_object_object_get(jobj, "routes");
    json_object *route = json_object_array_get_idx(routes, 0);
    json_object *sections = json_object_object_get(route, "sections");

    // Iterate through the different elements in section in the json_object and finds the duration
    for (int i = 1; i < places_len; ++i) {
        json_object *section = json_object_array_get_idx(sections, i - 1);
        json_object *summary = json_object_object_get(section, "travelSummary");
        travel_time[i] = json_object_get_int(json_object_object_get(summary, "duration"));

        json_object *arrival = json_object_object_get(section, "arrival");
        json_object *place = json_object_object_get(arrival, "place");
        json_object *orgloca = json_object_object_get(place, "originalLocation");

        // gets the latitude and longtitude of the current element in the json object and finds the location in places it
        // corresponds to, so that the location can get its dest_on_route set.
        const char *lat = json_object_get_string(json_object_object_get(orgloca, "lat"));
        const char *lng = json_object_get_string(json_object_object_get(orgloca, "lng"));
        for (int j = 0; j < places_len; ++j) {
            if (strncmp(lng, places[j].lng, 7) == 0 && strncmp(lat, places[j].lat, 7) == 0) {
                places[j].dest_on_route = i;
            }
        }
    }
    return travel_time;
}
int *route_time(location *places, char *transportation, char *apikey, size_t places_len) {
    char url[1000] = "https://router.hereapi.com/v8/routes?";
    add_if_api(url, apikey);

    // Checks if the locations in places isn't entirely empty, else the prgram is stopped
    for (int i = 0; i < places_len; ++i) {
        if (is_addr_empty(&places[i])) {
            printf("ERROR an address was entirely empty");
            exit(EXIT_FAILURE);
        }

        // calls are made so the missing information in all the location in places are filled
        else if (strcmp(places[i].place_name, places[0].place_name) == 0);
        else if (strncmp("home", places[i].place_name, 4)== 0) addr_to_geo(&places[i], apikey);
        else store_to_geo(&places[i], apikey, places[0].lat, places[0].lng);
    }
    for (int i = 0; i < places_len; ++i) {
        if (i == 0) strcat(url, "origin=");
        else if (i == places_len - 1) strcat(url, "destination=");
        else {
            strcat(url, "via");
            strcat(url, "=");
        }
        add_strings(url, 4, places[i].lat, ",", places[i].lng, "&");
    }
    add_strings(url, 2,"return=travelSummary&transportMode=", transportation);

    char *str = curlext_easy_fetch(url, "https");

    // Convets str to json_object
    json_object *jobj = json_tokener_parse(str);

    int *time_arr = json_to_traveltime(jobj, places, places_len);

    time_arr[0] = 0;
    time_arr[0] = sum_of_arr(time_arr, places_len);
    free(str);
    return time_arr;
}

void initialize_location(location *place) {
    strcpy(place->postalCode, "");
    strcpy(place->city,       "");
    strcpy(place->houseNumber,"");
    strcpy(place->street,     "");
    strcpy(place->state,      "");
    strcpy(place->county,     "");
    strcpy(place->lat,        "");
    strcpy(place->lng,        "");
    strcpy(place->place_name, "");
}

void clrscr() {
    system("@cls||clear");
}

char *get_address(location *place, AddressComponent i) {
    switch(i) {
        case STREET:        return place->street;
        case HOUSE_NUMBER:  return place->houseNumber;
        case CITY:          return place->city;
        case POSTAL_CODE:   return place->postalCode;
        case COUNTY:        return place->county;
        case STATE:         return place->state;
        default:
            printf("ERR out of index");
            exit(0);
    }
}

void set_address(location *place, AddressComponent i, char *str){
    switch (i) {
        case STREET:
            strcpy(place->street, str);
            break;
        case HOUSE_NUMBER:
            strcpy(place->houseNumber,str);
            break;
        case CITY:
            strcpy(place->city, str);
            break;
        case POSTAL_CODE:
            strcpy(place->postalCode, str);
            break;
        case COUNTY:
            strcpy(place->county, str);
            break;
        case STATE:
            strcpy(place->state, str);
            break;
        default:
            printf("ERR out of index");
            exit(0);
    }
}
