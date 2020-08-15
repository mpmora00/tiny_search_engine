/* 
 * index.c 
 * see REQUIREMENTS.md and DESIGN.md for more information.
 *
 * Maria Paula Mora, May 2020
 * Purpose: reads the document files produced by the TSE crawler, 
 * builds an index, and writes that index to a file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "hashtable.h"
#include "counters.h"
#include "memory.h" 
#include "pagedir.h"
#include "index.h"
#include "word.h"

/**************** Main function ****************/

int main(int argc, char *argv[]) {
    
    // if the wrong number of parameters is given
    if (argc != 3) {
        fprintf(stderr, "Error: Incorrect number of parameters given.\n");
        return 1; 
    }

    // parses arguments
    char *page_dir = argv[1];                       // pathname of a directory
    char *index_filename = argv[2];                       // pathname of a file

    // check validity of arguments
    
    // 1. checks to see if pageDirectory is indeed a Crawler-produced directory
    if (!is_crawler_directory(page_dir)) {
        fprintf(stderr, "Error: Directory given %s is not a valid directory\n", page_dir);
        return 1; 
    } 
    // 2. check to see if index file is a valid pathname
    if (!valid_filename(index_filename)) {
        fprintf(stderr, "Error: index file %s is not a valid pathname\n", page_dir);
        return 1; 
    } 

    // create a hashtable
    hashtable_t *hash_table = hashtable_new(900);
    if (hash_table == NULL) {
        fprintf(stderr, "Error creating a hashtable\n");
        return 2;
    }
    
    // initializes other modules
    int ret = index_build(page_dir, hash_table);

    // if there was any error building the index
    if (ret != 0) {
        // free up space
        index_delete(hash_table);
        // if index build returned an error
        return 3; 
    }

    // save the index into a file
    ret = index_save(index_filename, hash_table);
    
    // if there was any error saving the file
    if (ret != 0) {
        // free up space
        index_delete(hash_table);
        // if index build returned an error
        return 3; 
    }

    // free up space
    index_delete(hash_table);

    return 0; 
}
