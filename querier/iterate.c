/* 
 * iterate.c 
 * see iterate.h for more information
 *
 * Maria Paula Mora, May 2020
 * Purpose: create a iterating functions to be 
 * used during the iteration of a counter
 */


#include <stdio.h>
#include <stdlib.h>
#include "pair.h"
#include "counters.h"
#include "iterate.h"


/**************** Handle counters iterate ****************/
/* 
* used to merge two counters (the one with the smallest score gets merged)
* if one of the counters does not have it value will be zero 
*/
void counters_least(void *arg, const int key, const int count){
    // Get value from the second counter
    counters_t* second = (counters_t*)arg;
    int second_count = counters_get(second, key);

    // Keep only the smallest value
    int least;
    if (second_count < count) {
        least = second_count;
    }
    else {
        least = count;
    }

    // set the counter to the smallest value
    counters_set(second, key, least);
}

// add all the counters with non-zero valued counters 
void add_all(void *arg, const int key, const int count){
    counters_t* secondcounter = (counters_t*)arg;
    // if the counter is not zero
    if(count > 0){
        // add it to the other new counter
        counters_set(secondcounter, key, count);
    }

}

// used for counters iterate to add the counts between two counters
void counters_merge(void *arg, const int key, const int count) {

    counters_t* secondcounter = (counters_t*)arg;
    counters_set(secondcounter, key, counters_get(arg, key) + count); 
}

// calculate the size of the counter
void countersSize(void *arg, const int key, const int count) {
    // update the other counter 
    int* size = (int *)arg; 
    *size = *size+1;
}

// sort a counter in decreasing order
void sort_counter(void *arg, const int key, const int count) {
    pairs_t** list = (pairs_t**) arg;
    pairs_t* pair = new_pair(key,count);

    int size = score_get(list[0]);   // calculate the size of the array
    
    int i, p = 1; 
    // for every pair in the array
    for (i = 1; i < size; i++) {
        // check to see if where we are is smaller than our count
        if (count >= score_get(list[i])) {
            // save the index
            p = i;
            // exit the loop
            break;
        }
    }

    // move all data at right side of the array
    for (i = size - 1; i >= p && i > 1; i--) {
        list[i] = list[i-1];
    }

    // insert value at the proper position
    list[p]=pair;
}