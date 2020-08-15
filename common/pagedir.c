/* 
 * pagedir.c 
 *
 * Maria Paula Mora, April 2020
 * Purpose: outputs a page to the appropriate file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "memory.h"
#include "file.h"
#include "pagedir.h"

/**************** Local function ****************/
int char_counter(int num);

// check to see if directory given exists
bool directory_exists(char *page_dir){
    // string together the filename 
    char *filename =  count_malloc_assert(strlen(page_dir)+sizeof(char)+strlen(".crawler")+1, "Error allocating memory"); 
    sprintf(filename, "%s/%s", page_dir, ".crawler");

    // open the file
    FILE *fp_test = fopen(filename, "w");

    // check if file was opened correctly
    if(fp_test == NULL){
        // free the memory        
        count_free(filename);
        return false;
    }

    // free the memory
    count_free(filename);
    // close the file
    fclose(fp_test);
    return true;
}

// outputs a page to the appropriate file 
int pagesaver(webpage_t *page, char *page_dir, int current_id) {
    /* 
    * find the file name to save it as 
    * the filename is of form "pageDirectory/id" 
    * on NULL return from malloc/calloc you can print an error and exit with non-zero exit status.
    */
    char *filename = count_malloc_assert(strlen(page_dir)+sizeof(char)*(char_counter(current_id))+2, "Error allocating memory");
    sprintf(filename, "%s/%d", page_dir, current_id);

    FILE *fp_out = fopen(filename, "w");
    
    // check if file was opened correctly
    if (fp_out == NULL) { 
        // free the memory 
        count_free(filename);

        fprintf(stderr, "Error: File %s failed to be opened\n", filename);
        return 1;
    }

    /* 
    * The first line of the file is the URL
    * the second line of the file is the depth
    * the rest of the file is the page content (the HTML, unchanged) 
    */
    fprintf(fp_out, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));

    // free the memory
    count_free(filename);

    // close the file 
    fclose(fp_out); 

    return 0; 
}

// Verify whether dir is indeed a Crawler-produced directory
bool is_crawler_directory(char *dir){
    // string together the filename 
    char *filename =  count_malloc_assert(strlen(dir)+sizeof(char)+strlen(".crawler")+1, "Error allocating memory"); 

    if ((dir[strlen(dir) - 2]) == 47) {
        sprintf(filename, "%s%s", dir, ".crawler");
    }
    else {
        sprintf(filename, "%s/%s", dir, ".crawler");
    }

    // open the file
    FILE *fp_test = fopen(filename, "r");

    // check if file was opened correctly
    if(fp_test == NULL){
        // free the memory        
        count_free(filename);
        return false;
    }

    // free the memory
    count_free(filename);
    filename = NULL; 
    // close the file
    fclose(fp_test);
    return true;
}

// Check to see if file is a valid pathname
bool valid_filename(char *index_filename){
    // try to open the file
    FILE *fp_test = fopen(index_filename, "w");

    // check if file was opened correctly
    if(fp_test == NULL){
        // free the memory        
        return false;
    }

    // close the file
    fclose(fp_test);
    return true;
}

// create a wepage from a file
webpage_t* page_load(char *page_dir, int docID) {
    
    // string together the filename 
    char *curr_filename =  count_malloc_assert(strlen(page_dir)+sizeof(char)+char_counter(docID)+1, "Error allocating memory"); 
    sprintf(curr_filename, "%s/%d", page_dir, docID);
    
    // try to open the file
    FILE *curr_file = fopen(curr_filename, "r");

    // check if file was opened correctly
    if(curr_file == NULL){
        // free the memory  
        count_free(curr_filename);      
        return NULL;
    }

    // read the first two lines of the file (url, depth)
    char *url = freadlinep(curr_file);

    // check to see if the file was read correctly
    if (url == NULL) {
        // free the memory
        count_free(curr_filename); 
        // close the file
        fclose(curr_file);
        return NULL; 
    }

    char *second_line = freadlinep(curr_file);
    int depth;      // webpage depth

    
    // check if the file was read correctly
    if (second_line == NULL || sscanf(second_line, "%d", &depth) != 1) {
        // check if an interger can be read
        if (second_line != NULL) {
            // free the memory  
            count_free(second_line); 
        }

        // free the memory  
        count_free(url);
        count_free(curr_filename); 

        // close the file
        fclose(curr_file);
        return NULL;
    }


    // free the second line memory string
    free(second_line);

    // read the rest of the the file (depth)
    char *HTML = freadfilep(curr_file); 
    // check to see if the file was read correctly
    if (HTML == NULL) {
        // free the memory
        count_free(url);
        count_free(curr_filename); 
        // close the file
        fclose(curr_file);
        
        return NULL; 
    }

    // create a website from the strings
    webpage_t *webpage = webpage_new(url, depth, HTML);
    
    // free the memory  
    count_free(curr_filename); 

    // close the file
    fclose(curr_file);

    // return the website
    return webpage;  
}

/**************** Local function ****************/
/* 
* count the number of characters in a given parameter
* used to allocate memory for filename 
*
* static means it is not shown outside of file 
*/
int char_counter(int num) {
    // all numbers have at least 1 character
    int char_num = 0; 

    // check to what place we are looking at (tenth?)
    int place = 1; 

    // while the number is bigger the the place
    while (place <= num) {
        // increase number of character
        char_num +=1; 
        // increase place (tenth? hundreth? thousand?)
        place *= 10; 
    }
    return char_num; 
}