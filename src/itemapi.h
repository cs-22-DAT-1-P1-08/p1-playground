//
// Created by carl on 28/11/2022.
//

#ifndef P1_PLAYGROUND_ITEMAPI_H
#define P1_PLAYGROUND_ITEMAPI_H

/***
 */
int printcurl(char* url, char* storeID, char* key);

typedef struct item{
    double price;
    char* name;
    char* ean;
    double amount;

} item;



#endif //P1_PLAYGROUND_ITEMAPI_H
