/* 
 * iterate.h - header file for pair module
 *
 * Has a list of functions that can be utilized inside the function
 * counters_iterate to handle different actions wanted
 * 
 * Maria Paula Mora, April 2020
 */

#ifndef __ITERATE_H
#define __ITERATE_H

#include <stdio.h>
#include <stdbool.h>

/**************** counters_least ****************/
/* 
 * Used to merge two counters together
 * The smallest score between the two counters, is score that is added
 * Modifies the arg (which should be another counter)
 *
 * We return:
 *   nothing 
*/
void counters_least(void *arg, const int key, const int count);

/**************** add_all ****************/
/* 
 * Used to merge two counters together
 * Any zero valued counter is ignored and not added
 * Modifies the arg (which should be another counter)
 *
 * We return:
 *   nothing 
*/
void add_all(void *arg, const int key, const int count);

/**************** counters_merge ****************/
/* 
 * Used to merge two counters together
 * Adds the sum of the two counters together
 * Modifies the arg (which should be another counter)
 *
 * We return:
 *   nothing 
*/
void counters_merge(void *arg, const int key, const int count);

/**************** countersSize ****************/
/* 
 * Used to count how many counters are in a counter struct
 * Modifies the arg (which should be an int)
 * 
 * We return:
 *   nothing 
*/
void countersSize(void *arg, const int key, const int count);

/**************** sort_counter ****************/
/* 
 * Used to sort counters
 * Modifies the arg (which should be an array of pointer pairs)
 * For every counter, a new pair is created and inserted into the list
 * in decresing order
 * 
 * We return:
 *   nothing 
*/
void sort_counter(void *arg, const int key, const int count);

#endif // __ITERATE_H