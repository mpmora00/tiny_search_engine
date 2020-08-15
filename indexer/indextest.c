/* 
 * indextest.c 
 * see REQUIREMENTS.md and DESIGN.md for more information.
 *
 * Maria Paula Mora, May 2020
 * Purpose: load the index from the oldIndexFilename 
 * into an inverted-index data structure
 */

#include <stdio.h>
#include "hashtable.h"
#include "counters.h"
#include "index.h"
#include "pagedir.h"
#include "file.h"

/**************** Main function ****************/

int main(int argc, char *argv[]) {
    
    // if the wrong number of parameters is given
    if (argc != 3) {
        fprintf(stderr, "Error: Incorrect number of parameters given.\n");
        return 1; 
    }

    // parses arguments
    char *oldIndexFilename = argv[1];     // name of a file produced by the indexer
    char *newIndexFilename = argv[2];     // name of a file into which the index should be written

    // check validity of arguments
    
    /*
    * Checks to see if oldIndexFilename exists
    * try to open the file
    */
    FILE *fp_in = fopen(oldIndexFilename, "r");

    // check if file was opened correctly
    if(fp_in == NULL){
        fprintf(stderr, "Error opening first file\n");
        // free the memory        
        return 1;
    }
    else if (!valid_filename(newIndexFilename)) {
        fprintf(stderr, "Error: index file %s is not a valid pathname\n", newIndexFilename);
        return 1; 
    } 

    // create a hashtable
    hashtable_t *hash_table = hashtable_new(lines_in_file(fp_in));
    if (hash_table == NULL) {
        fprintf(stderr, "Error creating a hashtable\n");
        return 2;
    }
    
    // load the index from a file
    int ret = index_load(fp_in, hash_table);
    // check if it loaded correctly
    if (ret != 0) {
        fclose(fp_in);
        hashtable_delete(hash_table, NULL);
        return 3; 
    } 

    // close the files
    fclose(fp_in);

    // save the new index into a file
    ret = index_save(newIndexFilename, hash_table);
    
    // if there was any error saving the file
    if (ret != 0) {
        index_delete(hash_table);
        // if index build returned an error
        fprintf(stderr, "Error saving index into file\n");
        return 3; 
    }

    // free up memory
    index_delete(hash_table);

    return 0;
}