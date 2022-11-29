#include "coopapi.h"
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

//Enum only for Daglibrugsen and Coop365, because it's the only working Coop api storeIDs
enum storeNameToID{Daglibrugsen=0, Coop365};

int main(int argc, char *argv[]){
    char coopurl[100] = "https://api.cl.coop.dk/productapi/v1/product/";
    char coopapikey[50];
    int storeName;
    char *storeID = malloc(sizeof(char)*10000);
    switch (storeName) {
        case 0:
            strcpy(storeID, "1290");
            break;
        case 1:
            strcpy(storeID, "24165");
            break;
        case 2:
    }
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