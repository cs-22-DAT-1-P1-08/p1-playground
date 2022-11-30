//
// Created by carl on 28/11/2022.
//

#ifndef P1_PLAYGROUND_ITEMAPI_H
#define P1_PLAYGROUND_ITEMAPI_H

/***
 */
int printcurl(char* url, char* storeID);

typedef struct item{
    char* ean;
    double price;
    char* name;
    double amount;
} item;



#endif //P1_PLAYGROUND_ITEMAPI_H
