#ifndef CURLEXT_H
#define CURLEXT_H

#include <stdio.h>

typedef struct {
    char *memory;
    size_t size;
} MemoryStruct;

char* curlext_easy_fetch(char* url, char* protocol);

#endif