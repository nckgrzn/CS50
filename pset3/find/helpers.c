/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // verify that search value is positive
    if (n <= 0)
    {
        return false;
    }
    else
    {
        // while length of list > 0
        int upperbound = n;
        int lowerbound = 0;
        while (n > 0)
        {
            // look at middle of list
            int middle = (upperbound + lowerbound) / 2;
            // if number found, return true
            if (values[middle] == value)
            {
                return true;
            }
            // else if number higher, search left
            else
            {
                if (values[middle] < value)
                {
                    lowerbound = middle;
                    n = n / 2;
                }
                // else if number lower, search right
                else
                {
                    if (values[middle] > value)
                    {
                        upperbound = middle;
                        n = n / 2;
                    }
                }
            }
        }
        // return false
        return false;
    }
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // initialize swap counter
    int swaps;
    do
    {
       // set swaps to 0 before each iteration
       swaps = 0;
       // iterate through array
       for(int count = 0, sorted = 0; count < (n - 1); count++, sorted++)
        {
            // swap numbers that are not sorted correctly
            if (values[count] > values[count + 1])
            {
                int temp = values[count];
                values[count] = values[count + 1];
                values[count + 1] = temp;
                // tally swaps
                swaps++;
            }
        } 
    }
    // repeat until no swaps are made
    while (swaps != 0);
    return;
}
