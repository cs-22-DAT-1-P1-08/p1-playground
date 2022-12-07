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
} location;

/**
 * Takes in what parts of an address in the form of multiple strings in the location struct,
 * and inserts it into the url for the Here api:
 * https://developer.here.com/documentation/geocoding-search-api/dev_guide/index.html
 * Then from the address information given the rest of the location struct is filled in including
 * the latitude and longitude.
 *
 * @param place struct location which is both used as argument and is what is returned
 * @param apikey string which is used to get access to the here api
 */
void addr_to_geo(location *place, char* apikey);

/**
 * Returns an updated location element of store with full address and latitude and longitude
 * depending on which store of the place_name is closest to the given lng and lat
 * https://developer.here.com/documentation/geocoding-search-api/dev_guide/topics-api/code-discover-chain.html
 *
 * @param place struct location which is both used as argument and is what is returned
 * @param apikey string which is used to get access to the here api
 * @param lat the latitude of the position the stor has to be close to
 * @param lng the longitude of the position the store has to be close to
 */
void store_to_geo(location *store, char* apikey, char *lat, char *lng);

/**
 * Uses libcurl to calculate the time between multiple different location elements,
 * both between the individual destinations and the entire route,
 * by the way of transportation chosen to get the string with the information a call to the api_to_str is made.
 * @param places an array of elements of the struct location.
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
 * @return
 */
int *route_time(location *places, char *transportation, char* apikey, size_t places_len);

/**
 * Sets every string of the location struct place to an empty string.
 * @param place a location struct
 */
void initialize_location(location *place);

/**
 * A function which makes it possible to iterate through the address part of the location struct.
 * @param place the location element
 * @param i The part of the address which is to be returned as a string \n
 * 0: street.       \n
 * 1: houseNumber.  \n
 * 2: city.         \n
 * 3: postalCode.   \n
 * 4: county.       \n
 * 5: state.        \n
 * @return A string from place depending on which i is parsed.
 */
char *get_address(location *place, AddressComponent i);


#endif //UNTITLED_API_H
