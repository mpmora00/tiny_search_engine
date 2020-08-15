/* 
 * counters.c - CS50 'counters' module
 *
 * see counters.h for more information.
 *
 * Maria Paula, April 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "memory.h"

/**************** local types ****************/
typedef struct countersnode {
    int key;		      // integer for the key value
    int count;              // how many time has the key been called/what has it been set to
    struct countersnode *next;	      // link to next node
} countersnode_t;

/**************** global types ****************/
typedef struct counters {
    struct countersnode *head;	      // head of the list of items in counter
} counters_t;


/**************** local functions ****************/
/* not visible outside this file */
static countersnode_t *countersnode_new(const int key);

/**************** counters_new() ****************/
counters_t* counters_new(void)
{
    // allocate memory for the counter
    counters_t *counters = count_malloc(sizeof(counters_t)); 

    if (counters == NULL) {
        return NULL; // error allocating counters
    } 
    else {
        // initialize contents of counters structure
        counters->head = NULL;
        return counters;
    }
}

/**************** counters_add() ****************/
int counters_add(counters_t *ctrs, const int key)
{
    if (ctrs != NULL && key >= 0 ) { // check if counters is not null and key is positive
        
        // check the counters to see if key exists
        for (countersnode_t *node = ctrs->head; node != NULL; node = node->next ) {
            if (node->key == key) { // if the key exists
                node->count = node->count + 1;  // increment the counter
                return node->count; // return the count of the current node added
            }
        }
        // allocate a new node to be added to the list
        countersnode_t *new = countersnode_new(key);
        
        if (new != NULL) {                
            // add it to the head of the list
            new->next = ctrs->head;
            ctrs->head = new;
            return new->count;
        }
        else {
            return 0; // error 
        }
    }
    else {
        return 0; // control was null or key was negative
    }
}

/**************** countersnode_new ****************/
/* Allocate and initialize a countersnode */
// the 'static' modifier means this function is not visible 
// outside this file
static countersnode_t* countersnode_new(const int key)
{
    if (key >= 0) {
        // allocate memory for a countersnode
        countersnode_t *node = (countersnode_t *)count_malloc(sizeof(countersnode_t));

        if (node == NULL) {
            // error allocating memory for node; return error
            return NULL;
        } 
        else { // no error allocating memory
            // add the key and count values for the countersnode
            node->key = key;
            node->count = 1;
            node->next = NULL; // set the next node to null
            return node;
        }
    }
    else {
        return NULL; // wrong key was given
    }
}

/**************** counters_get() ****************/
int counters_get(counters_t *ctrs, const int key)
{
    if (key >= 0 && ctrs != NULL) {

        // try to find the key
        for (countersnode_t *node = ctrs->head; node != NULL; node = node->next ) {
            if (node->key == key) { // if the key exists
                return node->count; // the node to take out
            }
        }
        return 0; // key does not exist
    }
    else {
        return 0; // NULL counter or wrong key given
    }
}

/**************** counters_set() ****************/
bool counters_set(counters_t *ctrs, const int key, const int count) {
    if (ctrs != NULL && key >=0 && count>=0) {
         // try to find the key
        for (countersnode_t *node = ctrs->head; node != NULL; node = node->next ) {
            if (node->key == key) { // if the key exists
                node->count = count; // the node to take out
                return true; // the countersnode was changed
            }
        }
        // the key was not found in the counter set
        // allocate a new node to be added to the list with that key value
        countersnode_t *new = countersnode_new(key);
        
        // if a new countersnode was able to be created
        if (new != NULL) {      
            new->count = count; // assign count valuse
          
            // add it to the head of the list
            new->next = ctrs->head;
            ctrs->head = new;
            return true;
        }
        else {
            return false; // error 
        }
    }
    else {
        return false; // NULL counter, negative key or negative counter was given
    }
}


/**************** counters_print() ****************/
void counters_print(counters_t *ctrs, FILE *fp) {
    if (fp != NULL) {
        if (ctrs != NULL) {
            countersnode_t *node;

            if (ctrs->head != NULL) {
                // open brackers
                fprintf(fp, "{");

                // go through all of the nodes
                for (node = ctrs->head; node->next != NULL; node = node->next) {
                    // print this node as a comma separated item pair
                    fprintf(fp, "(%d=%d)", node->key, node->count);
                    fprintf(fp, ","); // add comma
                }
                // print last key=count pair
                fprintf(fp, "(%d=%d)", node->key, node->count);

                //closer brackets
                fprintf(fp, "}\n");
            }  
            else {
                fprintf(fp, "(null)\n"); // empty counters was given
            }
        } 
        else {
            fprintf(fp, "(null)\n"); // NULL counters was given
        }
    }
}

/**************** counters_iterate() ****************/
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count)) {
  if (ctrs != NULL && itemfunc != NULL) {
    
    // call itemfunc with arg, on each item
    for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->count); 
    }
  }
}

/**************** counters_delete() ****************/
void counters_delete(counters_t *ctrs) {
    if (ctrs != NULL) {
        for (countersnode_t *node = ctrs->head; node != NULL; ) {
            countersnode_t *next = node->next;	    // remember what comes next
            count_free(node);			    // free the node
            node = next;			    // and move on to next
        }
        count_free(ctrs); // free the counter
        ctrs = NULL; 
    }
}
