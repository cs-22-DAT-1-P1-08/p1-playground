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

char* to_lower(char *str) {
    int len = (int)strlen(str);
    char* result = calloc(len + 1, sizeof(char));
    for (int i = 0; i < len; i++){
        result[i] = tolower(str[i]);
    }
    return result;
}

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

int cmp_item_price(item_t **a, item_t **b) {
    // TODO: Include offer prices
    return (int)((*a)->price - (*b)->price);
}

item_t *find_cheapest_match(store_t *store, char* search_term) {
    char* lower_search_term = to_lower(search_term);

    item_t *cheapest = NULL;
    dlist_node_t *item_node = store->items->head;
    while (item_node != NULL) {
        printf("1 ");
        item_t *item = item_node->data;

        if (item == NULL || item->name == NULL) {
            item_node = item_node->next;
            continue;
        }
        printf("2 ");

        char* lower_item_name = to_lower(item->name);
        printf("3 ");
        if (strstr(lower_item_name, lower_search_term) != NULL) {
            printf(" - %s (%.2lf kr)\n", item->name, item->price);
            if (cheapest == NULL || cheapest->price > item->price) {
                cheapest = item;
            }
        }
        printf("4 ");
        free(lower_item_name);

        item_node = item_node->next;
        printf("5 \n");
    }
    printf("here %d\n", cheapest == NULL);

    free(lower_search_term);
    printf("also here\n");
    return cheapest;
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
