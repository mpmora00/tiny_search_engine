/* 
 * pagedir.h - header file for CS50 pagedir module
 *
 * Purpose: write the webpage to the pageDirectory with 
 * a unique document ID.
 *
 * Maria Paula Mora, April 2020
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include "webpage.h"

/**************** directoryexists ****************/
/* Check if a directory exists
 *
 * Caller provides:
 *   a page directory name
 * 
 * We return:
 *   true if the directory exists.
 *   false if the directory does not exist
 * 
 * A new file called .crawler will be found in your directory if it exists
 */
bool directory_exists(char *name_dir); 

/**************** pagesaver ****************/
/* Outputs a page to the appropriate file.
 *
 * Caller provides:
 *   a webpage with a valid HTML.
 *   an existent page directory name
 *   a positive current ID number
 * We return:
 *   0 if the file was written correctly.
 *   1 if the file was not opened correctly
 * 
 * Output: We save each explored webpage to a file, one file per page. 
 * We use a unique document ID as the file nam.
 * Within a file, we write
 *     the page URL on the first line,
 *     the depth of the page (where the seed is depth 0) on the second line,
 *     the page contents, beginning on the third line.
 */
int pagesaver(webpage_t *page, char *page_dir, int current_id);

/**************** is_crawler_directory ****************/
/* Verifies whether dir is indeed a Crawler-produced directory.
 *
 * Caller provides:
 *   A directory name
 * 
 * We return:
 *   true if the dir is indeed a Crawler-produced directory
 *   false if dir is not a Crawler-produced directory
 */
bool is_crawler_directory(char *dir);

/**************** valid_filename ****************/
/* Verifies whether the file name is a valid pathname
 *
 * Caller provides:
 *   A file name
 * 
 * We return:
 *   true if the file name is a valid pathname
 *   false if file name is not a valid pathname
 */
bool valid_filename(char *index_filename);

/**************** webpage_from_file ****************/
/* Create a wepage from a file
 *
 * Caller provides:
 *   the directory where the file is
 *   an integer index, which represents a file name
 * 
 * We return:
 *   NULL if there was an error opening/reading the file or saving the website
 *   webpage pointer if everything was done correctly
 * 
 * The function allocates memory for the file name and frees that memory
 * It also opens the file to read and closes it
 * If any error is present, any memory allocated is freed before exiting
 * 
 * Caller must remember to free the memory for the webside
 * if website_delete is not called, then the strings and the website 
 * will cause a memory leak
 */

webpage_t* page_load(char *page_dir, int docID);

/**************** char_counter ****************/
/* Find the number of characters in an integer
 *
 * Caller provides:
 *   an integer
 * 
 * We return:
 *      an integer (number of characters
 * 
 * Used as a helper function in many other files
 */
int char_counter(int num);

#endif // __PAGEDIR_H