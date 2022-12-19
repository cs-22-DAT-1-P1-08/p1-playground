#ifndef UNTITLED_API_H
#define UNTITLED_API_H

#include <stdio.h>
#include <json-c/json.h>

enum AddressComponent {
    STREET,
    HOUSE_NUMBER,
    CITY,
    POSTAL_CODE,
    COUNTY,
    STATE
};
typedef enum AddressComponent AddressComponent;

/**
 * Includes strings for
 * @param Address \n
 * state,\n county,\n city,\n street,\n postalCode,\n houseNumber.
 * @param Geo_coordinates \n
 * lat,\n lng
 * @param place_name
 */
typedef struct {
    //Address
    char state[18], county[32], city[25], street[37], postalCode[5], houseNumber[5];
    //Geo coordinates
    char lat[9], lng[9];
    //Distance from user
    int dest_on_route;
    //Place name
    char place_name[20];
} location_t;

/**
 * Takes in what parts of an address in the form of multiple strings in the location_t struct,
 * and inserts it into the url for the Here api:
 * https://developer.here.com/documentation/geocoding-search-api/dev_guide/index.html
 * Then from the address information given the rest of the location_t struct is filled in including
 * the latitude and longitude.
 *
 * @param place struct location_t which is both used as argument and is what is returned
 * @param apikey string which is used to get access to the here api
 */
void addr_to_geo(location_t *place, char* apikey);

/**
 * Returns an updated location_t element of store with full address and latitude and longitude
 * depending on which store of the place_name is closest to the given lng and lat
 * https://developer.here.com/documentation/geocoding-search-api/dev_guide/topics-api/code-discover-chain.html
 *
 * @param place struct location_t which is both used as argument and is what is returned
 * @param apikey string which is used to get access to the here api
 * @param lat the latitude of the position the stor has to be close to
 * @param lng the longitude of the position the store has to be close to
 */
void store_to_geo(location_t *store, char* apikey, char *lat, char *lng);

/**
 * Uses libcurl to calculate the time or lengthbetween multiple different location_t elements,
 * both between the individual destinations and the entire route,
 * by the way of transportation chosen to get the string with the information a call to the api_to_str is made.
 * @param places an array of elements of the struct location_t.
 * @param transportation a string which describes which mode of transportation is used:
 * \n car,
 * \n truck,
 * \n pedestrian,
 * \n bicycle,
 * \n scooter,
 * \n taxi
 * \n bus
 * @param apikey string which is used to get access to the here api.
 * @param places_len amount of destinations.
 * @param output_duration if the output is wished to be the duration or the length of the route
 * @return
 */
int *route_time(location_t *places[], char *transportation, char* apikey, size_t places_len, int output_duration);

/**
 * Sets every string of the location_t struct place to an empty string.
 * @param place a location_t struct
 */
void initialize_location(location_t *place);

/**
 * A function which makes it possible to iterate through the address part of the location_t struct.
 * @param place the location_t element
 * @param i The part of the address which is to be returned as a string \n
 * 0: street.       \n
 * 1: houseNumber.  \n
 * 2: city.         \n
 * 3: postalCode.   \n
 * 4: county.       \n
 * 5: state.        \n
 * @return A string from place depending on which i is parsed.
 */
char *get_address(location_t *place, AddressComponent i);

/**
 * Switch case, which copies with strcpy on to the element of place which correlates with the iterator i
 * from the string str.
 *
 * @param place an element of struct location_t which get its element updated to str.
 * @param i decides which element is to be replaced with str.
 * 0: street.       \n
 * 1: houseNumber.  \n
 * 2: city.         \n
 * 3: postalCode.   \n
 * 4: county.       \n
 * 5: state.        \n
 * @param str string is the source which an element is updated with.
 */

void set_address (location_t *place, AddressComponent i, char *str);


int is_addr_empty(location_t *place);

#endif //UNTITLED_API_H
