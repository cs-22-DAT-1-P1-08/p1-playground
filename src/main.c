#include "coopapi.h"
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
typedef enum storeid{"fakta"=1290, "Dagli'brugsen"=1043} storeid;

int main(int argc, char *argv[]){
    char coopurl[100] = "https://api.cl.coop.dk/productapi/v1/product/";
    char coopapikey[50];

    printf("Input API key: ");
    scanf("%s", coopapikey);
    char *str = malloc(sizeof(char)*10000);
    if (str==NULL){
        perror("Could not create big string!");
        exit(1);
    }


    coopapi_to_itemstr(url, "https", str);

    printf("%s\n\n", str);

    return 0;
}