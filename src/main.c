#include <stdio.h>
#include "sorting_functions.h"

int main()
{
    varer dkk, x, ib, EAN, u, qa, ie;
    int item_name[] = {9,3,48,91,6101,30,795};


    int n = sizeof(item_name)/sizeof(item_name[0]);
    printf("Shopping list:");
    print_array(item_name, n);

    radix_sort(item_name, n);

    printf("The cheapest is the following:");
    print_array(item_name);
    return 0;
}


