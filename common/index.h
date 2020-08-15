/* 
 * index.h - header file for CS50 pagedir module
 *
 * Purpose:  contains all the logic for saving 
 * and loading index files
 *
 * Maria Paula Mora, May 2020
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include "hashtable.h"

/**************** index_build ****************/
/* Creates an index from a directory (a hashtable of a set of counters)
 * Each counter has a file_id as key and a word count as values
 *
 * Caller provides:
 *   A directory (method checks if it's a crawler made directory)
 *   A hashtable pointer
 * 
 * Returns:
 *   1 if there was any error
 *   0 if the function exited correctly
 */
int index_build(char *page_dir, hashtable_t *hash_table);

/**************** index_save ****************/
/* Saves an index into a file
 * One line per word, one word per line
 * each line provides the word and one or more (docID, count) pairs, in the format
 * word docID count [docID count]…
 *
 * Caller provides:
 *   A name of a file to save the index in
 *   A hashtable pointer (the index)
 * 
 * Returns:
 *   1 if there was any error
 *   0 if the function exited correctly
 */
int index_save(char *filename, hashtable_t* hash_table);

/**************** index_delete ****************/
/* Free's up all of the memory allocated by the index
 * 
 * Caller provides:
 *   A hashtable where the index was created
 * 
 */
void index_delete(hashtable_t* hash_table);

/**************** index_load ****************/
/* Load an index from a file into an inverted-index data structure
 * 
 * Caller provides:
 *   A hashtable where they want the index to be saved
 *   A file to read the index from
 * 
 * Returns:
 *      1 if an error was encountered
 *      0 if the program exited correctly 
 * 
 */
int index_load(FILE *fp_in, hashtable_t* hash_table);

#endif // __INDEX_H