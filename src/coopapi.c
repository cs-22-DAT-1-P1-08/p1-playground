//
// Created by carl on 28/11/2022.
//
#include "itemapi.h"
#include "util/dlist.h"
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <curl/curl.h>



int printcurl(char* url, char* storeID){
    char fullurl[100];
    strcpy(fullurl, url);
    strcat(url, storeID);

    char fullkey[100] = "Ocp-Apim-Subscription-Key: ";
    char* key= getenv("CoopAPIKey");
    if (key == NULL){
        fprintf(stderr, "Failed to find CoopAPIKey in environment variables");
        exit(EXIT_FAILURE);
    }
    strcat(fullkey, key);
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, fullurl);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, fullkey);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        /* disables SSL security, very bad, but as of know we got no clue how to fix our SSL or SSH problem*/
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        res = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);
    char items[100000] = res;
    json_object* root = json_tokener_parse(res);
    return (int)res;
}


void curl_to_struct(){

}