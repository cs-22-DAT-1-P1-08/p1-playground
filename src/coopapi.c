//
// Created by carl on 28/11/2022.
//
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
int main(int argc, char *argv[]){
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.cl.coop.dk/productapi/v1/product/1290");
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Ocp-Apim-Subscription-Key: 443332aed9904c8f902861f77ad68d42");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        /* disables SSL security, very bad, but as of know we got no clue how to fix our SSL or SSH problem*/
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        res = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);
    return (int)res;
}