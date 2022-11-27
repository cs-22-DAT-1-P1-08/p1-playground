#include <curl/curl.h>
#include "curlext.h"

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

char* curlext_easy_fetch(char* url, char* protocol) {
    CURL *curl_handle;
    CURLcode res;

    MemoryStruct chunk;
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    if (!curl_handle) {
        fprintf(stderr,"[-] Failed Initializing Curl\n");
        exit(-1);
    }
    curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_DEFAULT_PROTOCOL, protocol);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    /* disables SSL security, very bad, but as of know we got no clue how to fix our SSL or SSH problem*/
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0);

    res = curl_easy_perform(curl_handle);

    if (res != CURLE_OK) {
        fprintf(stderr,"[-] Could not fetch webpage\n[+] Error : %s\n", curl_easy_strerror(res));
        exit(-2);
    }
    char *str_dest = malloc(sizeof(char) * chunk.size);
    strcpy(str_dest, (char *)chunk.memory);

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();

    return str_dest;
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