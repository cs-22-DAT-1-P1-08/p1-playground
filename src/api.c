#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <stdarg.h>

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
    printf("%s", str);

    //TODO get lat and lng and rest of address info from json and update the geocode struct
    free(str);
}

void store_to_geo(geocode *store, char* apikey, char *lat, char *lng) {
    char url[200] = "https://discover.search.hereapi.com/v1/discover?";
    add_if_api(url, apikey);
    add_strings(url,  7, "at=", lat, ",", lng, "&limit=1&", "q=", store->place_name);

    char *str = malloc(sizeof(char) * 10000);
    api_to_str(url, "https", str);
    printf("%s", str);

    //TODO get lat and lng and rest of address info from json and update the geocode struct
    free(str);
}

int *route_time(geocode *places, char *transportation, char *apikey, size_t places_len) {
    char url[200] = "https://wps.hereapi.com/2/findsequence.json?";
    add_if_api(url, apikey);
    for (int i = 0; i < places_len; ++i) {
        if (is_addr_empty(&places[i])) {
            printf("ERROR an address was entirely empty");
            exit(EXIT_FAILURE);
        }
        else if (i == 0) addr_to_geo(&places[i], apikey);
        else store_to_geo(&places[i], apikey, places[0].lat, places[0].lng);

    }
    for (int i = 0; i < places_len; ++i) {
        if (i == 0) strcat(url, "start=");
        else if (i == places_len - 1) strcat(url, "end=");
        else {
            strcat(url, "destination");
            itoa(i, url, 10);
            strcat(url, "=");
        }
        add_strings(url, 8, places[i].place_name, ";", places[i].place_name, ";", places[i].lat, ",", places[i].lng, "&");
    }
    add_strings(url, 3,"improveFor=time&mode=fastest;", transportation, ";");
    char *str = malloc(sizeof(char) * 10000);
    api_to_str(url, "https", str);
    printf("%s", str);

    //TODO get time spent between every destination and the final time from the json output
    free(str);
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
    place->postalCode   = "";
    place->city         = "";
    place->houseNumber  = "";
    place->street       = "";
    place->state        = "";
    place->county       = "";
    place->lat          = "";
    place->lng          = "";
    place->place_name   = "";
}

void clrscr() {
    system("@cls||clear");
}

char *get_address(geocode *place, int i) {
    switch(i) {
        case 0: return place->street;
        case 1: return place->houseNumber;
        case 2: return place->city;
        case 3: return place->postalCode;
        case 4: return place->county;
        case 5: return place->state;
        default:
            printf("ERR out of index");
            exit(0);
    }
}
