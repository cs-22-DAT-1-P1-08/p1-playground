// C implementation of Radix Sort
#include "util/dlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum AmountUnit {
    GRAMS,
    LITERS,
    PIECES
};

typedef enum AmountUnit quantity_type;

typedef struct{
double pris;
char* navn;
bool is_bargain;
char* EAN;
quantity_type unit;
double quantity_amount;
bool is_ecological;
}varer;


int get_max(int item_name[], int n);

void radix_sort(int item_name[], int n);

void print_array(int item_name[]);

#ifndef P1_PLAYGROUND_SORTING_FUNCTIONS_H
#define P1_PLAYGROUND_SORTING_FUNCTIONS_H

#endif //P1_PLAYGROUND_SORTING_FUNCTIONS_H
