/* 
 * pair.c 
 * see pair.h for more information
 *
 * Maria Paula Mora, May 2020
 * Purpose: create a pair of key-score values
 * Used to sorts counters rapidly
 */


#include <stdio.h>
#include <stdlib.h>
#include "pair.h"
#include "memory.h"

/**************** Key score pairs ****************/
typedef struct pair {
    int score;
    int docID;
} pairs_t;

// create a new pair struc
pairs_t* new_pair(int docID, int score) {
    // allocate memory for the counter
    pairs_t *pair = count_malloc(sizeof(pairs_t)); 

    if (pair == NULL) {
        return NULL; // error allocating counters
    } 
    else {
        // initialize contents of counters structure
        pair->docID = docID;
        pair->score = score; 
        return pair;
    }
}

// get the score of the pair
int score_get(pairs_t *pair){
    if (pair != NULL) {
        return pair->score;
    }
    else {
        return 0; // NULL pair
    }
}

// get the docID of the pair
int docID_get(pairs_t *pair){
    if (pair != NULL) {
        return pair->docID; 
    }
    else {
        return 0; // NULL pair
    }
}