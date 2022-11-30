#include "itemapi.h"
#include "util/dlist.h"
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

//Enum only for Daglibrugsen and Coop365, because it's the only working Coop api storeIDs
enum storeNameToID{Daglibrugsen=0, Coop365};

int main(void){
    char coopurl[100] = "https://api.cl.coop.dk/productapi/v1/product/";

    //Daglibrugsen
    printcurl(coopurl, "1290");
    //coop 365
    printcurl(coopurl, "24165");


    return 0;
}