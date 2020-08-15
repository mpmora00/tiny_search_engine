/* 
 * querier.c 
 * see REQUIREMENTS.md and DESIGN.md for more information.
 *
 * Maria Paula Mora, May 2020
 * Purpose: reads the index file produced by the TSE Indexer, 
 * and page files produced by the TSE Crawler, and answers 
 * search queries submitted via stdin
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "hashtable.h"
#include "counters.h"
#include "index.h"
#include "pagedir.h"
#include "memory.h"
#include "file.h"
#include "word.h"
#include "pair.h"
#include "iterate.h"

// provided by the lab
int fileno(FILE *stream);

/**************** Define local function ****************/
static void prompt(void);

// handles parsing the query
static void divide_string(char* words[], char *line, int count);
static int word_counter(char *line);
static bool parse_query(char* words[], int count);

// handles finding the query
static bool satisfy_query(hashtable_t* index, counters_t* res_or, char *words[], int count);
static bool solve_andsequence(hashtable_t* index, counters_t** res_and, counters_t* res_or, char *words[], int* i, int count);

// handles printing results
static void print_results(counters_t *result, char* pageDirectory);
static char* getURL(char *pageDirectory, int docID); 

/**************** Main function ****************/
int main(int argc, char *argv[]) {
    
    // if the wrong number of parameters is given
    if (argc != 3) {
        fprintf(stderr, "Error: Incorrect number of parameters given.\n");
        return 1; 
    }

    // parses arguments
    char *pageDirectory = argv[1];     // pathname of a directory produced by the Crawler
    char *indexFilename = argv[2];     // pathname of a file produced by the Indexer

    // check validity of arguments

    // 1. checks to see if pageDirectory is indeed a Crawler-produced directory
    if (!is_crawler_directory(pageDirectory)) {
        fprintf(stderr, "Error: Directory given %s is not a valid directory\n", pageDirectory);
        return 1; 
    } 

    /* 
    * 2. check to see if index file is a valid pathname
    * by opening the file to prep it for loading 
    */

    FILE *fp_in = fopen(indexFilename, "r");

    // check if file was opened correctly
    if(fp_in == NULL){
        fprintf(stderr, "Error opening %s file\n", indexFilename);
        // free the memory        
        return 1;
    }

    // create a hashtable to load the index in
    hashtable_t *index = hashtable_new(lines_in_file(fp_in));
    if (index == NULL) {
        fprintf(stderr, "Error creating a hashtable\n");
        return 2;
    }
    
    // load the index from a file
    int ret = index_load(fp_in, index);
    // check if it loaded correctly
    if (ret != 0) {
        fclose(fp_in);
        hashtable_delete(index, NULL);
        return 3; 
    } 

    // close the files
    fclose(fp_in);
    
    char *query;
    // ask for next query
    prompt(); 
     
    // read search queries from stdin, one per line, until EOF.
    while ((query = freadlinep(stdin)) != NULL) {
        
        /* count how many words are in the array
        * to initialize the array size */
        int count = word_counter(query);

        // ignore an empty string or a string with special characters
        if (count != 0) {
             // break a single string into an array of strings
            char* words[count];
            divide_string(words, query, count); 

            
            /* clean and parse each query according to the syntax described below.
            * if the query syntax is somehow invalid, print an error message
            * do not perform the query, and prompt for the next query
            */
            if (parse_query(words, count)) {

                // create a counter which will hold the query result
                counters_t *res_or = counters_new(); 
                if (res_or == NULL) {
                    fprintf(stderr, "Error creating a counter\n");
                    return 2;
                }
                /* use the index to identify the set of documents that
                * satisfy the query, as described below */
                if (satisfy_query(index, res_or, words, count)) {
                    
                    // print the result
                    print_results(res_or, pageDirectory);
                }
                else {
                    fprintf(stderr, "There was a problem initializing structures):\n"); 
                }
                counters_delete(res_or);

            }
            
        }
        // ask for next query
        prompt(); 
        free(query);
        query = NULL;
    }
    // add an extra space after Query? when it's over
    printf("\n");

    // free up memory
    index_delete(index);
    index = NULL; 

    return 0;
}


/**************** Handling the query ****************/
// print a prompt for an interactive user
static void prompt(void) {
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

// break a single string into an array of strings
static void divide_string(char* words[], char *line, int count) {

    int i = 0;
    // for every character in the line
    for (char *cp = line; *cp != '\0'; cp++) {
        // if its an alphabetical letter
        if (isalpha(*cp)){
            // add it as the start of the string
            words[i] = cp;
            i++;
            // while it's still reading that word
            while(*cp != '\0' && isalpha(*cp)) {
                cp++;
            }
            // if it is a space, then add a null
            if (isspace(*cp)){
                *cp = '\0';
            }
            // if not, then skip to the next pointer
            else{
                break;
            }   
        }
    }
}

// calculate the number of words in a string
static int word_counter(char *line) {
     // check how many words are in the query
    int count = 0;

    if(strlen(line) == 0) {
        return 0; 
    }

    // check the first line
    if (isalpha(line[0])) {
        count++; 
    }

    // for every character in a string
    for (int i =1; i< strlen(line); i++) {
        
        // if any digit or special character was found
        if (!isalpha(line[i]) && !isspace(line[i])) {
            // if the query syntax is somehow invalid, print an error message
            fprintf(stderr, "Error: bad character '%c' in query\n", line[i]);
            return 0;
        }
        // if the line has an space and then another letter (meaning a word start)
        else if (isalpha(line[i]) && isspace(line[i -1])) {
            // increase the count
            count++;  
        }
    }
    return count; 
}

/* Clean and parse each query according to the syntax described below.
* if the query syntax is invalid, print an error message, do not perform the query
* print the ‘clean’ query for user to see.
*/
static bool parse_query(char* words[], int count) {

    // translate all upper-case letters on the input line into lower-case
    for (int i=0; i < count; i++) {

        if (!NormalizeWord(words[i])) {
            fprintf(stderr, "Error: cannot normalize the word %s\n", words[i]); 
            return false; 
        }
    }

    // print the ‘clean’ query for user to see.
    fprintf(stdout, "Query:");
    for (int i = 0; i < count; i++) {
        fprintf(stdout, " %s", words[i]); 
    }    
    fprintf(stdout, "\n");

    /* check if the query follows the correct syntax */

    char* or  = "or\0";
    char* and  = "and\0";
    // 1. check the first
    if (strcmp(words[0], and) == 0 || strcmp(words[0], or) == 0 ) {
        // if the query syntax is somehow invalid, print an error message
        fprintf(stderr, "Error: '%s' cannot be first\n", words[0]);
        return false;
    }

    // 2. check the last word
    if (strcmp(words[count - 1], or) == 0 || strcmp(words[count - 1], and) == 0 ) {
        // if the query syntax is somehow invalid, print an error message
        fprintf(stderr, "Error: '%s' cannot be last\n", words[count - 1]);
        return false;
    }

    // 3. check every other words (without the first and the last)
    for (int i = 1; i < count - 1; i++) {
        // if the words is "and"
        if (strcmp(words[i], and) == 0 || strcmp(words[i], or) == 0) {
            if (strcmp(words[i - 1], or)  == 0 || strcmp(words[i + 1], or)  == 0) {
                // if the query syntax is somehow invalid, print an error message
                fprintf(stderr, "Error: 'and' and 'or' cannot be adjacent\n");
                return false;
            }
            else if (strcmp(words[i - 1], and)  == 0 || strcmp(words[i + 1], and)  == 0) {
                // if the query syntax is somehow invalid, print an error message
                fprintf(stderr, "Error: 'and' and 'or' cannot be adjacent\n");
                return false;
            }
        }
    }
    return true; 
}

/**************** Finding the query ****************/

// use the index to identify the set of documents that satisfy the query
static bool satisfy_query(hashtable_t* index, counters_t* res_or, char *words[], int count) {
    counters_t* res_and = counters_new();       // current  andsequence counters

    // check if they where initialized correctly
    if (res_and == NULL || res_or == NULL) {
        fprintf(stderr, "Could not initialize structures.\n");
        return false;
    }

    // for every word in the array
    for (int i = 0; i <= count ; i++) {

        /* if the last word was already seen or we just saw the word or
        * copy everything from res_and to res_or */
        if (i == count || strcmp(words[i], "or") == 0) {
            
            // merge them by adding their counters
            counters_iterate(res_and, res_or, counters_merge);

            // reset the res_and counter
            counters_delete(res_and);
            res_and = counters_new();
            if (res_and == NULL) {
                return false;
            }
        }

        // solve the and sequence
        else if (!solve_andsequence(index, &res_and, res_or, words, &i, count)) {
            // free memory
            counters_delete(res_and);
            return false; 
        }
    }

    // free memory
    counters_delete(res_and);

    return true; 
}

// solve the and sequence
static bool solve_andsequence(hashtable_t* index, counters_t** res_and, counters_t* res_or, char *words[], int* i, int count) {
    
    // this will hold the result of the and sequence
    counters_t* result = counters_new();
    if (result == NULL) {
        return false;
    }
    // add everything that is already in the and sequence into this new result
    counters_iterate(*res_and, result, add_all);

    counters_t* temp = hashtable_find(index, words[*i]);

    /* if the counter was not found, ignore rest (since one has no counters for that word,
    * the intersection of the and sequence will not exist) */
    if (temp == NULL) {
        // delete the res_and restart it
        counters_delete(result);
        result = NULL;
        result = counters_new();
        if (result == NULL) {
            return false;
        }
            
        // Move to the next andsequence start
        while (*i < count - 1 && strcmp(words[*i + 1], "or") == 0) { 
            i++;
        }
    }
    // if it was found
    else {
        /* the start of a new andsequence
        * this means a new word was just seen, either after an "or" or the
        * first word itself */
        if (*i == 0 || strcmp(words[*i - 1], "or") == 0) {
            // add them all of them 
            counters_iterate(temp, result, add_all);
            
        }
        // solve the rest of the andsequence
        else if (strcmp(words[*i], "and") != 0) {

            // used to delete all zero counter items when solving an andsequence
            counters_t* connection = counters_new();
            if (connection == NULL) {
                // free memory
                counters_delete(result);
                result = NULL;
                return false;
            }

            /* calculate the rest of the sequence
            * calculate the smallest values between the current word and res_and values */
            counters_iterate(result, temp, counters_least);
            counters_iterate(temp, result, counters_least);

            counters_iterate(result, connection, add_all);

            // make res_and = connection
            counters_delete(result);
            result = connection;
        }
        // free the pointer
        temp = NULL; 
    }

    // delete what was currently in the res_and
    counters_delete(*res_and);
    *res_and = counters_new();
    if (res_and == NULL) {
        return false;
    }

    // add everything that was in the result to res_and
    counters_iterate(result, *res_and, add_all);
    counters_delete(result);
    result = NULL; 
    return true;
}

/**************** Print the results ****************/

static void print_results(counters_t *result, char* pageDirectory) {
    // calculate the number of matches found
    int size = 0; 
    counters_iterate(result, &size, countersSize);

    // prints the number of documents
    if (size == 0) {
        // if no documents satisfy the query
        fprintf(stdout, "No documents match.\n");
    }
    else {
        fprintf(stdout, "Matches %d documents (ranked):\n", size); 

        // rank the scores
        pairs_t* ordered[size + 1];

        // be able to give the iteration the size of the array
        pairs_t* size_pair = new_pair(size + 1, size + 1); 
        if (size_pair == NULL) {
            fprintf(stderr, "Error: problem initializing a structure");
            return;
        }
        // set the first element to the size
        ordered[0] = size_pair; 

        // set everything in the array to zero
        for (int i = 1; i < size + 1; i++) {
            ordered[i] = 0; 
        }

        counters_iterate(result, ordered, sort_counter);

        // find the number of hcracaters of the biggest integer
        
        // since size is > 0, and the it's size+1, then there must at least be an index
        int max = char_counter(score_get(ordered[1])); 
        max ++;

        for (int i = 1; i < size + 1; i++) {
            char* url = getURL(pageDirectory, docID_get(ordered[i]));
            fprintf(stdout, "score %*d doc %3d: %s\n", max, score_get(ordered[i]), docID_get(ordered[i]), url); 
            
            // free the memory
            free(ordered[i]); 
            free(url); 
        }

        // free the size
        free(ordered[0]);
    }

    // separate section
    fprintf(stdout, "-----------------------------------------------\n");
}

static char* getURL(char *pageDirectory, int docID) {
    
    char *filename = count_malloc_assert(strlen(pageDirectory)+sizeof(char)*(char_counter(docID))+2, "Error allocating memory");
    sprintf(filename, "%s/%d", pageDirectory, docID);

    // try to open the file
    FILE *fp_in = fopen(filename, "r");        

    // check if file was opened correctly
    if(fp_in == NULL) {
        // free the memory        
        return NULL;
    }
    
    char *URL = freadlinep(fp_in); 

    // close the file
    fclose(fp_in);
    count_free(filename);
    filename = NULL;
    return URL;
}