#include "location_api.h"
#include "location_api_p.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl/curl.h"
#include <stdarg.h>
#include <json-c/json.h>

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

void add_geocode_to_url(geocode *place, char* url, void f(char *, char *, int)) {
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

int is_addr_empty(geocode *place) {
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

void addr_to_geo(geocode *place, char* apikey) {
    char url[200] = "https://geocode.search.hereapi.com/v1/geocode?";
    add_if_api(url, apikey);
    strcat(url, "q=");
    add_geocode_to_url(place, url, &str_for_url);

    char *str = malloc(sizeof(char) * 10000);
    api_to_str(url, "https", str);
    fill_geocode(place, str, 0);
    free(str);
}

void replace_address(geocode *place, json_object *address, int args,...) {
    va_list valist;
    va_start(valist, args);
    for (int i = 0; i < args; ++i) {
        const char *str = (char *)va_arg( valist, const char* );
        const char *source = json_object_get_string(json_object_object_get(address, str));
        set_address(place, i, (char *) source);
    }
    va_end(valist);
}

void fill_geocode(geocode *place, char *str, int item_num){
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
void store_to_geo(geocode *store, char* apikey, char *lat, char *lng) {
    char url[200] = "https://browse.search.hereapi.com/v1/browse?";
    add_if_api(url, apikey);
    add_strings(url, 6, "at=", lat, ",", lng, "&categories=600-6300-0066&name=", store->place_name);
    char *str = malloc(sizeof(char) * 100000);
    api_to_str(url, "https", str);
    int i;
    json_object *find_payment = NULL;
    json_object *items = json_tokener_parse(str);
    items = json_object_object_get(items, "items");
    for ( i = 0; i < json_object_array_length(items); ++i) {

        find_payment = json_object_array_get_idx(items, i);
        find_payment = json_object_object_get(find_payment, "payment");
        if (find_payment != NULL) break;

    }
    fill_geocode(store, str , i);
    free(str);
}

int *route_time(geocode *places, char *transportation, char *apikey, size_t places_len) {
    char url[1000] = "https://wps.hereapi.com/2/findsequence.json?";
    add_if_api(url, apikey);
    for (int i = 0; i < places_len; ++i) {
        if (is_addr_empty(&places[i])) {
            printf("ERROR an address was entirely empty");
            exit(EXIT_FAILURE);
        }
        else if (strncmp("home",places[i].place_name,4)== 0) addr_to_geo(&places[i], apikey);
        else store_to_geo(&places[i], apikey, places[0].lat, places[0].lng);

    }
    for (int i = 0; i < places_len; ++i) {
        if (i == 0) strcat(url, "start=");
        else if (i == places_len - 1) strcat(url, "end=");
        else {
            strcat(url, "destination");
            char arr_numb[5];
            itoa(i, arr_numb, 10);
            strcat(url, arr_numb);
            strcat(url, "=");
        }
        add_strings(url, 6, places[i].place_name, ";", places[i].lat, ",", places[i].lng, "&");
    }
    add_strings(url, 2,"improveFor=time&mode=fastest;", transportation);

    char *str = malloc(sizeof(char) * 100000);
    api_to_str(url, "https", str);

    int *travel_time = malloc(sizeof (int) *places_len);
    json_object *jjobj = json_tokener_parse(str);
    json_object *results = json_object_object_get(jjobj, "results");
    json_object *result = json_object_array_get_idx(results, 0);
    travel_time[0] = json_object_get_int(json_object_object_get(result, "time"));
    json_object *interconnections = json_object_object_get(result, "interconnections");
    for (int i = 1; i < places_len; ++i) {

        json_object *interconnection = json_object_array_get_idx(interconnections, i-1);
        travel_time[i] = json_object_get_int(json_object_object_get(interconnection, "time"));
    }
    free(str);
return travel_time;

}

//https://curl.se/libcurl/c/getinmemory.html

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


void api_to_str(char *url, char *protocol, char *str_dest) {
    CURL *curl;
    CURLcode res;

    MemoryStruct chunk;
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);
    /* init the curl session */
    curl = curl_easy_init();

    /* specify URL to get */
    if (!curl) {
        fprintf(stderr,"[-] Failed Initializing Curl\n");
        exit(-1);
    }
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, protocol);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    /* disables SSL security, very bad, but as of know we got no clue how to fix our SSL or SSH problem*/
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    res = curl_easy_perform(curl);

    clrscr();

    if (res != CURLE_OK) {
        fprintf(stderr,"[-] Could Not Fetch Webpage\n[+] Error : %s\n",curl_easy_strerror(res));
        exit(-2);
    }
    str_dest = realloc(str_dest, sizeof(char) * chunk.size);
    strcpy(str_dest, (char *)chunk.memory);

    /* cleanup curl stuff */
    curl_easy_cleanup(curl);
    free(chunk.memory);
    /* we are done with libcurl, so clean it up */
    curl_global_cleanup();
}

void initialize_geocode(geocode *place) {
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

char *get_address(geocode *place, AddressComponent i) {
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
void set_address(geocode *place, AddressComponent i, char *str){
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
