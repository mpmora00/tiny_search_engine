/* 
 * pair.h - header file for pair module
 *
 * A "pair set" is holds a key - score pair
 * Used to sorts counters rapidly
 * 
 * Maria Paula Mora, April 2020
 */

#ifndef __PAIR_H
#define __PAIR_H

#include <stdio.h>
#include <stdbool.h>

/**************** global types ****************/
typedef struct pair pairs_t;  // opaque to users of the module

/**************** functions ****************/

/**************** FUNCTION ****************/
/* Create a new pair with a set key-score value structure.
 *
 * We return:
 *   pointer to a new pair; NULL if error. 
 * Caller is responsible for:
 *   later freeing the pointer;
 */
pairs_t* new_pair(int docID, int score);

/**************** score_get ****************/
/* Return current value of the score in the pair
 *
 * Caller provides:
 *   valid pointer to a pair
 * We return:
 *   current value of the score
 *   0 if pair is NULL  
 * Note:
 *   pair is unchanged as a result of this call.
 */
int score_get(pairs_t *pair); 

/**************** docID_get ****************/
/* Return current value of the docID in the pair
 *
 * Caller provides:
 *   valid pointer to a pair
 * We return:
 *   current value of the docID
 *   0 if pair is NULL  
 * Note:
 *   pair is unchanged as a result of this call.
 */
int docID_get(pairs_t *pair); 


#endif // __PAIR_H