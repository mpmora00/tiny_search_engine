/* 
 * set.c 
 * see set.h for more information.
 *
 * Maria Paula Mora, April 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "memory.h"


/**************** local types ****************/
typedef struct setnode {
    void *item;		  
    char *key;
    struct setnode *next;	      // link to next node
} setnode_t;

/**************** global types ****************/
typedef struct set {
    setnode_t *head;	      // head of the list of items in the set
} set_t;

/**************** local functions ****************/
/* not visible outside this file */
static setnode_t *setnode_new(const char*key, void *item);

/**************** set_new() ****************/
set_t* set_new(void) {
    set_t *set = count_malloc(sizeof(set_t));

    if (set == NULL) {
        return NULL; // error allocating set
    } else {
        // initialize contents of set structure
        set->head = NULL;
        return set;
    }
}

/**************** set_insert() ****************/
bool set_insert(set_t *set, const char *key, void *item) {
    if (set != NULL && key != NULL && item != NULL) {
        
        // check the set to see if key exists
        for (setnode_t *node = set->head; node != NULL; node = node->next ) {
            if (strcmp(node->key, key) == 0) { // compare the current key with the key given
                return false; 
            }
        }

        // allocate a new node to be added to the list
        setnode_t *new = setnode_new(key, item);
        if (new != NULL) {
             // add new item to the head of the list
            new->next = set->head;
            set->head = new; 

            return true; // new item was inserted
        }
        else { // something went wrong 
            return false; 
        }
    }
    else { // a parameter was null
        return false; 
    }
}

/**************** setnode_new() ****************/
static setnode_t* setnode_new(const char*key, void *item)
{
    if (key != NULL && item != NULL) { 
        setnode_t *node = (setnode_t *)count_malloc(sizeof(setnode_t)); // allocate the memory for this new node

        if (node != NULL) { // if the memory was allocated succesfully
            node->key = (char *)count_malloc((strlen(key)+1)*sizeof(char)); // allocate memory for the key
            strcpy(node->key, key); // copy the key
             
            node->item = item; // set the item
            node->next = NULL;
        }
        return node;
    }
    else {
        return NULL; // a parameter is NULL
    }
}

/**************** set_find() ****************/
void *set_find(set_t *set, const char *key) {


    if (set != NULL && key != NULL && set->head != NULL) {
        // check the set to see if key exists
        for (setnode_t *current = set->head; current != NULL; current = current->next) {
            if (strcmp(current->key, key) == 0) {  // compare the current key with the key given
                return current->item; // if we found it, return the item
            }
        }
        return NULL; // key was not found
    }
    else {
        return NULL; // a parameter is null
    }
}

/**************** set_print() ****************/
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item) ) {
    if (fp != NULL) { // check if the file is open
        if (set != NULL) { // check if the set is NULL

            if (set->head != NULL) {
                fputc('{', fp); // opening bracket
                
                if (itemprint != NULL) {  
                    // print a comma-separated list of items surrounded by {brackets}
                    // current item being printed
                    setnode_t *node;
                    // go through every item in the set
                    for (node = set->head; node->next != NULL; node = node->next) {
                        // print this node
                        (*itemprint)(fp, node->key, node->item);
                        fputc(',', fp);
                    }
                    (*itemprint)(fp, node->key, node->item); // print last item
                }
                else {
                    fputc(' ', fp); // print a set with no items
                }
                fputs("}", fp); // closing bracker
            }
            else {
                fputs("(null)\n", fp); // print null
            }
        }
        else {
            fputs("(null)\n", fp); // print null
        }
    }
}

/**************** set_iterate() ****************/
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) ) {
    if (set != NULL && itemfunc != NULL) {
        // on each item of the set
        for (setnode_t *node = set->head; node != NULL; node = node->next) {
            (*itemfunc)(arg, node->key, node->item);  //call itemfunc 
        }
    }
}

/**************** set_delete() ****************/
void set_delete(set_t *set, void (*itemdelete)(void *item) ) {
    if (set != NULL) {
        // on each item of the set
        for (setnode_t *node = set->head; node != NULL;) {
            if (itemdelete != NULL) {		    // if possible...
                (*itemdelete)(node->item);      //call itemfunc 
            }
            setnode_t *next = node->next;	    // remember what comes next
            count_free(node->key);              // free all the key strings
            count_free(node);			        // free the node
            node = next;			            // and move on to next

        }
        count_free(set);    // free the set 
        set = NULL;         // make it NULL 
    }
}
