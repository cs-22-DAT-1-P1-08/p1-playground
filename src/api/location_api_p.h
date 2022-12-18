#ifndef P1_PLAYGROUND_LOCATION_API_P_H
#define P1_PLAYGROUND_LOCATION_API_P_H

#include "location_api.h"

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

/**
 * Take information from json_object, specific from address and its elements
 * (street, city,...) and put the information back into place's, which is of the struct
 * location_t elements (street, city,...)
 * @param place  use as output of the struct location_t
 * @param address json_object as have string with all address location_t information
 * @param args has the amount of arguments passed with the stdard.h library
 * @param ... the strings of categories in json which is to replace place's strings
 */

void replace_address(location_t *place, json_object *address, int args,...);

/**
 * Initiate a json_object with a string, then we scoope the string/object to get the element address,
 * address is passed into set_address where place's address elements get updated.
 * Then the json_object scoope the string/object to get the element position,
 * lat and lng is taken from postion in the json_object, and passed into place's elements (lat and lng)
 * @param place location_t struct which elements get updated
 * @param str is a string in json format which is used to initialize a json_object
 * @param item_num is the item in the array of different location_ts which is to be used
 */

void fill_location (location_t *place, char *str, int item_num);

/**
 * Dives through the json object jobj to find the time or length of the entire route and between the individuel location_ts
 * in places.
 * @param jobj is the json object which have the string being looked throgh
 * @param places the destinations on the route which is updated with which number on the route they got
 * @param places_len the amount of destinations on the route
 * @return the travel time between the different destinations.
 */
int *json_to_traveltime(json_object *jobj, location_t* places, size_t places_len, int output_duration);

#endif //P1_PLAYGROUND_LOCATION_API_P_H
