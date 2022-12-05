#include <stdio.h>
#include "sorting_functions.h"
#include "api/coop_api.h"
#include "store.h"
#include <string.h>
#include "store.h"
#include <stdlib.h>
#include "api/tjek_api.h"
#include "sorting_functions.h"
#include <curl/curl.h>
#include <locale.h>
#include <wchar.h>

// This  function gives maximum value in array[]
int get_max(int item_name[], int n)
{
    int max = item_name[0];
    for (int i = 1; i < n; i++){
        if (item_name[i] > max)
            max = item_name[i];
    }
    return max;
}

int cmp_item_price(item_t *a, item_t *b) {
    // TODO: Include offer prices
    return (int)(a->price - b->price);
}

item_t *find_cheapest_match(store_t *store, char* search_term) {
    dlist_t results;
    dlist_node_t *item_node = store->items->head;
    while (item_node != NULL) {
        item_t *item = item_node->data;
        // TODO: make string comparison case insensitive
        if (item != NULL && strstr(item->name, search_term) != NULL) {
            // add to results
            dlist_add(&results, item);
        }

        item_node = item_node->next;
    }
    item_t *result[results.count];
    dlist_fill_array(&results, (void **) result);
    //TODO: radix sort continue
    //radix_sort(result, results.count);
    qsort(result, results.count, sizeof(result[0]), cmp_item_price);
    return result[0];
}

// Main Radix Sort, sort function
/*
void radix_sort(item_t *item_name[], int count)
{
    int i, digit_place = 1;
    int result[count]; // resulting array
    // Find the largest number to know number of digits
    int largest_num = get_max(item[], count);

    //we run loop until we reach the largest digit place
    while(largest_num / digit_place > 0){

        int count[10] = {0};
        //Store the amount of keys
        for (i = 0; i < count; i++)
            count[ (item_name[i] / digit_place) % 10]++;

        // Change count[i] so that count[i] now contains actual
        //  position of this digit in result[]
        //  Working similar to the counting sort algorithm
        for (i = 1; i < 10; i++)
            count[i] += count[i - 1];

        // Build the resulting array
        for (i = count - 1; i >= 0; i--)
        {
            result[count[(item_name[i] / digit_place) % 10 ] - 1] = item_name[i];
            count[ (item_name[i] / digit_place) % 10 ]--;
        }

        // Now main array item_sort[] contains sorted
        // numbers according to current digit place
        for (i = 0; i < count; i++)
            item_name[i] = result[i];

        // Move to next digit place
        digit_place *= 10;
    }
} */

// Function to print array "item_name"
void print_array(int item_name[])
{
}

int cheapest_price (int item_name[], int n)
{

}
