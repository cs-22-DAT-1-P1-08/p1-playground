#ifndef UNTITLED_API_H
#define UNTITLED_API_H

#include <stdio.h>

typedef struct {
    char *memory;
    size_t size;
} MemoryStruct;

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
    char *state, *county, *city, *street, *postalCode, *houseNumber;
    //Geo coordinates
    char *lat, *lng;
    //Place name
    char *place_name;
} geocode;

/**
 * Takes in what parts of an address in the form of multiple strings in the geocode struct,
 * and inserts it into the url for the Here api:
 * https://developer.here.com/documentation/geocoding-search-api/dev_guide/index.html
 * Then from the address information given the rest of the geocode struct is filled in including
 * the latitude and longitude.
 *
 * @param place struct geocode which is both used as argument and is what is returned
 * @param apikey string which is used to get access to the here api
 */
void addr_to_geo(geocode *place, char* apikey);

/**
 * Returns an updated geocode element of store with full address and latitude and longitude
 * depending on which store of the place_name is closest to the given lng and lat
 * https://developer.here.com/documentation/geocoding-search-api/dev_guide/topics-api/code-discover-chain.html
 *
 * @param place struct geocode which is both used as argument and is what is returned
 * @param apikey string which is used to get access to the here api
 * @param lat the latitude of the position the stor has to be close to
 * @param lng the longitude of the position the store has to be close to
 */
void store_to_geo(geocode *store, char* apikey, char *lat, char *lng);

/**
 * Uses libcurl to calculate the time between multiple different geocode elements,
 * both between the individual destinations and the entire route,
 * by the way of transportation chosen to get the string with the information a call to the api_to_str is made.
 * @param places an array of elements of the struct geocode.
 * @param transportation a string which describes which mode of transportation is used:\n
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
int *route_time(geocode *places, char *transportation, char* apikey, size_t places_len);

/**
 * Uses libcurl to make get request through a passed url, the output of the request is through a helping function
 * WriteMemoryCallback to take the output and copy it into a string
 * @param url a string with the url which libcurl is to make a request with.
 * @param protocol the protocol for which the site utilize.
 * @param str_dest the string which the output of the get request is parsed to.
 */
void api_to_str(char *url, char *protocol, char *str_dest);

/**
 * Sets every string of the geocode struct place to an empty string.
 * @param place a geocode struct
 */
void initialize_geocode(geocode *place);

/**
 * A function which makes it possible to iterate through the address part of the geocode struct.
 * @param place the geocode element
 * @param i The part of the address which is to be returned as a string \n
 * 0: street.       \n
 * 1: houseNumber.  \n
 * 2: city.         \n
 * 3: postalCode.   \n
 * 4: county.       \n
 * 5: state.        \n
 * @return A string from place depending on which i is parsed.
 */
char *get_address(geocode *place, int i);

/**
 * Uses strarg library to add unlimited arguments to str_dest
 * @param str_dest the string which is added to
 * @param len amount of arguments passed
 * @param ... the (hopefully) strings being added to str_dest
 */
void add_strings(char *str_dest, int args,...);

/**
 * Clears the console
 */
void clrscr();

#endif //UNTITLED_API_H
