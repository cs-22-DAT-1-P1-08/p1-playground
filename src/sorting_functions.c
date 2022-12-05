#include <stdio.h>
#include "sorting_functions.h"
#include <string.h>

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

// Main Radix Sort, sort function
void radix_sort(int item_name[], int n)
{
    int i, digit_place = 1;
    int result[n]; // resulting array
    // Find the largest number to know number of digits
    int largest_num = get_max(item_name, n);

    //we run loop until we reach the largest digit place
    while(largest_num / digit_place > 0){

        int key_am[10] = {0};
        //Store the amount of keys
        for (i = 0; i < n; i++)
            key_am[ (item_name[i] / digit_place) % 10]++;

        // Change count[i] so that count[i] now contains actual
        //  position of this digit in result[]
        //  Working similar to the counting sort algorithm
        for (i = 1; i < 10; i++)
            key_am[i] += key_am[i - 1];

        // Build the resulting array
        for (i = n - 1; i >= 0; i--)
        {
            result[key_am[(item_name[i] / digit_place) % 10 ] - 1] = item_name[i];
            key_am[ (item_name[i] / digit_place) % 10 ]--;
        }

        // Now main array item_sort[] contains sorted
        // numbers according to current digit place
        for (i = 0; i < n; i++)
            item_name[i] = result[i];

        // Move to next digit place
        digit_place *= 10;
    }
}

// Function to print array "item_name"
void print_array(int item_name[])
{
    printf(" %d", item_name[0]);
}

int cheapest_price (int item_name[], int n)
{
    varer item;

    printf("");


}
