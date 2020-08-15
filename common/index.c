/* 
 * index.c 
 *
 * Maria Paula Mora, May 2020
 * Purpose: contains all the logic for saving 
 * and loading index files
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "counters.h"
#include "hashtable.h"
#include "pagedir.h"
#include "word.h"
#include "file.h"

/**************** Local functions ****************/
static void itemprint(void *arg, const char *key, void *item);
static void counters_printing(void *arg, const int key, const int count);
static void counters_deleting(void *item); 

/**************** index_build ****************/

// build an index from a directory
int index_build(char *page_dir, hashtable_t* hash_table) {

    int docID = 1; 

    webpage_t *webpage;

     // while the webpage can be created and the file can be opened
    while ((webpage = page_load(page_dir, docID)) != NULL) {
        //while there are more words in a website
        int pos = 0;
        char *word;
        while ((word = webpage_getNextWord(webpage, &pos)) != NULL) {

            /* Ignore words with fewer than three characters
            * Ignore words with with non alphabetic characters
            * Normalize the world */
            if (strlen(word) >= 3 && NormalizeWord(word)) { 

                counters_t *ctrs = hashtable_find(hash_table, word);
                // if hashtable doesn't contain word
                if (ctrs == NULL) {
                    // create a counters
                    ctrs = counters_new();
                    if (ctrs == NULL) {
                        fprintf(stderr, "Creating a counter failed\n");
                        return 1;
                    }
                    // add word to hashtable 
                    if (!hashtable_insert(hash_table, word, ctrs)) {
                        fprintf(stderr, "Error inserting into hashtable\n");
                        return 1;
                    }
                }
                // add docID to counter (increment count or start at 1)
                if (counters_add(ctrs, docID) == 0) {
                    fprintf(stderr, "Error adding to counter\n");
                    return 1;
                }
            }
            free(word); 
        } 
        //ncrease the docID
        docID += 1; 
        // free memory
        word = NULL; 
        webpage_delete(webpage);
    }

    return 0; 
}

/**************** index_save ****************/

// save an index into a file
int index_save(char *filename, hashtable_t* hash_table) {
    // try to open the file
    FILE *fp_in = fopen(filename, "w");

    // if file was not opened correctly
    if(fp_in == NULL){
        return 1;
    }

    // iterate over every item in the hashtable and print it
    hashtable_iterate(hash_table, fp_in, itemprint);

    // close the file 
    fclose(fp_in); 

    return 0; 
}

/**************** index_delete ****************/

// free's up all of the memory from the index
void index_delete(hashtable_t* hash_table) {
    hashtable_delete(hash_table, counters_deleting);
}

/**************** index_load ****************/

// load an index from a file into an inverted-index data structure
int index_load(FILE *fp_in, hashtable_t* hash_table) {
    int line_num = lines_in_file(fp_in); 
    int ret, key, count;
    
    // for every line in the file
    for (int i=0; i<line_num; i++) { 
        char *word; 
        // read the word
        if ((word = freadwordp(fp_in)) == NULL) {
            fprintf(stderr, "Incorrect file format in line %d.", i);
            return 1; 
        }
        
        // create a counter
        counters_t *ctrs = counters_new(); 
        if (ctrs == NULL) {
            fprintf(stderr, "Creating a counter failed\n");
            return 1;
        }

        // while there are more pairs in the file line
        while ((ret = fscanf(fp_in, " %d %d", &key, &count)) == 2) {
            // set the counter to that value
            if (!counters_set(ctrs, key, count)) {
                fprintf(stderr, "Error setting counters\n");
                return 1;
            }        
        }

        // insert this word into the hashtable
        if (!hashtable_insert(hash_table, word, ctrs)) {
            fprintf(stderr, "Error inserting into hashtable\n");
            return 1;
        }

        // free the memory
        free(word); 
        word = NULL; 
    }

    return 0; 
}



/**************** Local functions ****************/

/* Helper Function
* casts the item into a counter and deletes the counter
*/
static void counters_deleting(void *item) {
    counters_t *ctrs = item;    // cast
    counters_delete(ctrs);      // delete
}

/* Helper Function
* print the hashtable (helper)
*/
static void itemprint(void *arg, const char *key, void *item) {
    FILE *fp_in = arg;
    counters_t *ctrs = item;

    // print the key
    fprintf(fp_in, "%s", key);

    // iterate through each counter
    counters_iterate(ctrs, fp_in, counters_printing);
    
    // print an end of the line for each key 
    fprintf(fp_in, "\n");
}

/* Helper Function
*  print the counter item 
*/
static void counters_printing(void *arg, const int key, const int count) {
    FILE *fp_in = arg;
    // print the id and the count number
    fprintf(fp_in, " %d %d", key, count);
}
