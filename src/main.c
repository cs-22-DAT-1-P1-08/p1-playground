#include "itemapi.h"
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

//Enum only for Daglibrugsen and Coop365, because it's the only working Coop api storeIDs
enum storeNameToID{Daglibrugsen=0, Coop365};

int main(void){
    char coopurl[100] = "https://api.cl.coop.dk/productapi/v1/product/";
    char coopapikey[50] = "2170983e33714f1c93997346fbc5ae29";
    int storeName;
    char *storeID = malloc(sizeof(char)*10000);
    //Daglibrugsen
    printcurl(coopurl, "1290", coopapikey);
    //coop 365
    printcurl(coopurl, "24165", coopapikey);

    //printf("Input API key: ");
    //scanf("%s", coopapikey);
    char *str = malloc(sizeof(char)*10000);
    if (str==NULL){
        perror("Could not create big string!");
        exit(1);
    }


    //coopapi_to_itemstr(url, "https", str);

    printf("%s\n\n", str);
    free(storeID);
    free(str);
    return 0;
}