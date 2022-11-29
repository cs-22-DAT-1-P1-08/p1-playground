#ifndef P1_PLAYGROUND_API_P_H
#define P1_PLAYGROUND_API_P_H

typedef struct {
    char *memory;
    size_t size;
} MemoryStruct;

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
 * Uses libcurl to make get request through a passed url, the output of the request is through a helping function
 * WriteMemoryCallback to take the output and copy it into a string
 * @param url a string with the url which libcurl is to make a request with.
 * @param protocol the protocol for which the site utilize.
 * @param str_dest the string which the output of the get request is parsed to.
 */
void api_to_str(char *url, char *protocol, char *str_dest);

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
char *get_address(geocode *place, AddressComponent i);

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
 * geocode elements (street, city,...)
 * @param place  use as output of the struct geocode
 * @param address json_object as have string with all address location information
 * @param args has the amount of arguments passed with the stdard.h library
 * @param ... the strings of categories in json which is to replace place's strings
 */

void replace_address(geocode *place, json_object *address, int args,...);

/**
 * Initiate a json_object with a string, then we scoope the string/object to get the element address,
 * address is passed into set_address where place's address elements get updated.
 * Then the json_object scoope the string/object to get the element position,
 * lat and lng is taken from postion in the json_object, and passed into place's elements (lat and lng)
 * @param place Geocode struct which elements get updated
 * @param str is a string in json format which is used to initialize a json_object
 */

void fill_geocode (geocode *place, char *str);

/**
 * Switch case, which copies with strcpy on to the element of place which correlates with the iterator i
 * from the string str.
 *
 * @param place an element of struct geocode which get its element updated to str.
 * @param i decides which element is to be replaced with str.
 * @param str string is the source which an element is updated with.
 */

void set_address (geocode *place, AddressComponent i, char *str);



#endif //P1_PLAYGROUND_API_P_H
