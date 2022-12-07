#ifndef P1_PLAYGROUND_API_P_H
#define P1_PLAYGROUND_API_P_H

#include "api.h"

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
 * location elements (street, city,...)
 * @param place  use as output of the struct location
 * @param address json_object as have string with all address location information
 * @param args has the amount of arguments passed with the stdard.h library
 * @param ... the strings of categories in json which is to replace place's strings
 */

void replace_address(location *place, json_object *address, int args,...);

/**
 * Initiate a json_object with a string, then we scoope the string/object to get the element address,
 * address is passed into set_address where place's address elements get updated.
 * Then the json_object scoope the string/object to get the element position,
 * lat and lng is taken from postion in the json_object, and passed into place's elements (lat and lng)
 * @param place location struct which elements get updated
 * @param str is a string in json format which is used to initialize a json_object
 * @param item_num is the item in the array of different locations which is to be used
 */

void fill_location (location *place, char *str, int item_num);

/**
 * Switch case, which copies with strcpy on to the element of place which correlates with the iterator i
 * from the string str.
 *
 * @param place an element of struct location which get its element updated to str.
 * @param i decides which element is to be replaced with str.
 * @param str string is the source which an element is updated with.
 */

void set_address (location *place, AddressComponent i, char *str);



#endif //P1_PLAYGROUND_API_P_H