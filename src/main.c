#include "api.h"
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

int main() {
    char url[200] = "https://geocode.search.hereapi.com/v1/geocode?q=hadsundvej%2044,%20aalborg&apiKey=";
    char apikey[100];
    scanf("%s", apikey);
    add_strings(url, 1, apikey);
    char *str = malloc(sizeof(char) * 100000);
    if (str == NULL) {
        perror("could not create big string");
        exit(1);
    }
    api_to_str(url, "https", str);

    printf("%s\n\n", str);

    return 0;
}