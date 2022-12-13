#ifndef CURLEXT_H
#define CURLEXT_H

#include <stdio.h>

typedef struct {
    char *memory;
    size_t size;
} MemoryStruct;


char* curlext_easy_fetch(char* url, char* protocol);
char* curlext_easy_pfetch(char* url, char* protocol, char* post_fields, struct curl_slist *headers);



#endif