/* 
 * hashtable.c - CS50 'hashtable' module
 *
 * see hashtable.h for more information.
 *
 * Maria Paula, April 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "set.h"
#include "memory.h"
#include "jhash.h"

/**************** global types ****************/
typedef struct hashtable {
    set_t **htable; // make the pointer to the array
    int num_slots; 
} hashtable_t;

/**************** functions ****************/

/**************** hashtable_new ****************/
hashtable_t *hashtable_new(const int num_slots) {
    // allocate memory for a new hashtable structure
    hashtable_t *new = (hashtable_t *)count_malloc(sizeof(hashtable_t));

    if (new == NULL && num_slots >0) {
        return NULL; // error allocating set
    } else {
        // initialize contents of set structure
        new->htable = count_calloc(num_slots, sizeof(set_t*));
        new->num_slots = num_slots; // save the size of the array
    }
    return new; // return the hashtable structure
}

/**************** hashtable_insert ****************/
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht != NULL && item != NULL && key != NULL ) {
        unsigned long i = JenkinsHash(key, ht->num_slots); // get the hash
        
        set_t **array = ht->htable; // save the array
        
        // if a key has never been hased here, create a new set
        if (array[i] == NULL) {
            set_t *set = set_new(); // create a set
            if (set == NULL) {
                return false; //error
            }
            else {
                array[i] = set; // if it's not null, add this set into the array
            }
        }
        // once a set already exists in this hash location
        // insert into that set they key,item pair
        bool res = set_insert(array[i], key, item); 
    
        // if the key already existed it will not and it will return false
        // if a new item was inserted, will return true
        return res; 

    }
    else {
        return false;  // wrong parameters given
    }
}


/**************** hashtable_find ****************/
void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht != NULL && key != NULL) {
        unsigned long i = JenkinsHash(key, ht->num_slots); // get the hash

        set_t **array = ht->htable; // save the array
        if (array != NULL) {
            return set_find(array[i], key); // find the key
        }
        else {
            return NULL; 
        }
    } 
    else {
        return NULL; // wrong parameters given
    }
}


/**************** hashtable_print ****************/
/* Print the whole table; provide the output file and func to print each item. */
void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp != NULL) { // check if the file is open
        if (ht != NULL) { // check if the hastable is NULL

            // for every slot in the the array
            for (int i=0; i<(ht->num_slots); i++) {
                set_t *hashSlot = ht->htable[i]; // save the current hash slot

                if (hashSlot != NULL) {
                    if (itemprint != NULL) {  
                        set_print(hashSlot, fp, itemprint);
                        //  Handled by set_print:
                        // one line per hash slot, with no items, if NULL itemprint.
                        // one line per hash slot, listing (key,item) pairs in that slot
                        fputs("\n", fp); // line brake
                    }
                }
                else {
                    fputs("(null)\n", fp); // print null
                }
            }
        }
        else {
            fputs("(null)\n", fp); // print null
        }
    }
}

/**************** hashtable_iterate ****************/
void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) ) {
    if (ht != NULL && itemfunc != NULL) {
        // for every slot in the the array
        for (int i=0; i<(ht->num_slots); i++) {
            set_t *hashSlot = ht->htable[i]; // save the current hash slot

            if (hashSlot != NULL) { // if the slot if has a set inside
                set_iterate(hashSlot, arg, itemfunc); // iterate through that set
            }
        }
    }
}


/**************** hashtable_delete ****************/
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) ) {
    if (ht != NULL) { // if the hastable give is NULL do nothing

        // for every slot in the array
        for (int i=0; i<(ht->num_slots); i++) {
            set_t *hashSlot = ht->htable[i]; // save the current hash slot (a set)

            if (hashSlot != NULL) { // if this set was initialized
                set_delete(hashSlot, itemdelete); // delete the set
            }
        }
        count_free(ht->htable); // free the array
        free(ht); // free the hashtable
        ht = NULL; 
    }
}