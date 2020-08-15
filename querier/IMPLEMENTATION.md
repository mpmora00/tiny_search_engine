# CS50 Lab 3 Implementation of Querier
## Maria Paula Mora, CS50 May 2020

### Pseudocode for each object/components/functions 

Some functions that are important for querier are:

#### divide_string
1. For each character in the string
    2. if the character is a space, convert it into null
2. For each word (ending in null) allocate it into an array of words

#### parse_query
1. For each word in the query
    2. NormalizeWord
3. Print the 'clean' query
4. Check if the query follows the correct syntax
    5. if the first or the last word is an "and" or an "or" 
        6. return an error
    7. For every other word in the query
        8. check if "and" and "or" are adjacent

#### satisfy_query
1. Create two counters (res_and and res_or)
2. For every word in the query (using an integer i)
    3. if the word is "or" or we already seen the last word (the sences has already been read completely)
        4. merge current andsequence with the orsequence result
    5. else call solve_andsequence()
10. move all of res_or into the resulting counter and free all necessary memory

#### solve_andsequence
1. Find the counter for the current word and copy them to andsequence counter
2. if the word was not found
    3. delete the entire and sequence because there will be no intersection between the words
    4. loop until the next word is an or
4. else
    5. if it's the first word or if the word was found after an "or" (start of a new and sequence)
        6. add the entire counter that was found to the result 
    7. else if it's not an "and"
        6. Find the counter for the current word and find the intersection (intersection = the smallest counter out of the two)     

#### print_results
1. Calculate hw many matches where found in the resulting counter
2. If size = 0 print "No document match" 
3. else print the number of matches
    4. Order the resulting counter 
        5. Create and array of pair (a new struct) pointers
        6. Add an extra slot at the beginning of the array which holds a pair whose score and docID is the size of the array
        7. Iterate through counter, create a pair from each counter, and insert it into the array in an organized way
    5. Print the pairs as "score # doc #: url"
6. free any memory 
7. print an extra dividing line
            
#### Querier
1. Execute from a command line
2. Process and validate command-line parameters
3. Initialize a hashtable data structure data structure which will be used to create the inverted index
4. Read from the standard input
    5. Call `divide_string`
    6. If `parse_query`
        7. Call `satisfy_query`
        8. Call `print_results`
7. Free any other memory
9. Call `index_delete(index)`

In summary, the job of the querier is to reads the index file produced by the TSE Indexer,and page files produced by the TSE Crawler, and answers search queries submitted via stdin.

### Function Parameters
`querier.c` and `querier.c` run from the command line with the following syntax:
``` bash
./querier pageDirectory indexFilename
```
- where pageDirectory is the pathname of a directory produced by the Crawler
- where indexFilename is the pathname of a file produced by the Indexer

Afterwards, it promps the user for queries to look for in these documents. 

Since both of these functions utilized functions from the ``common.a`` lib for its implementations, then the parameters are very important to mention. You can see common's [../common/README.md](README.md) for more in-depth definitions of these functions.

#### Pair Structure
In [README.md](README.md), it was discussed that in order to sort a counter by its score, a pair struct was created in `pair.h` and `pair.c` Some important functions for this struct are discused below. 

``` c
/* Create a new pair with a set key-score value structure.
 *
 * We return:
 *   pointer to a new pair; NULL if error. 
 * Caller is responsible for:
 *   later freeing the pointer;
 */
pairs_t* new_pair(int docID, int score);

/* Return current value of the score in the pair
 *
 * Caller provides:
 *   valid pointer to a pair
 * We return:
 *   current value of the score
 *   0 if pair is NULL  
 */
int score_get(pairs_t *pair); 

/* Return current value of the docID in the pair
 *
 * Caller provides:
 *   valid pointer to a pair
 * We return:
 *   current value of the docID
 *   0 if pair is NULL  
 */
int docID_get(pairs_t *pair); 
```

#### Iterate Function
In [README.md](README.md), it was discussed that the files `iterate.h` and `iterate.c` are important for iterating through counters. Some important functions used for this, are discused below. 

``` c
/* 
 * Used for the "andsequence"
 * 
 * Used to merge two counters together
 * The smallest score between the two counters, is score that is added
 * Modifies the arg (which should be another counter)
*/
void counters_least(void *arg, const int key, const int count);

/* 
 * Used to merge two counters together
 * Any zero valued counter is ignored and not added
 * Since that means that the word was not found and should not be printed
 * Modifies the arg (which should be another counter)
*/
void add_all(void *arg, const int key, const int count);

/* 
 * Used for the "orsequence"
 * 
 * Used to merge two counters together
 * Adds the sum of the two counters together
 * Modifies the arg (which should be another counter)
*/
void counters_merge(void *arg, const int key, const int count);

/* 
 * Used to count the number of matches that the query found after searching
 * 
 * Used to count how many counters are in a counter struct
 * Modifies the arg (which should be an int)
*/
void countersSize(void *arg, const int key, const int count);

/* 
 * Used for organizing the counters in a decreasing order
 * in order to print the scores of the doc ID as a result 
 *
 * Modifies the arg (which should be an array of pointer pairs)
 * For every counter, a new pair is created and inserted into the list
 * in decresing order
*/
void sort_counter(void *arg, const int key, const int count);
```

### Data structures utilized 
These definitons come from the Lab 3 webpage (https://www.cs.dartmouth.edu/~cs50/Labs/Lab3/)

#### Hashtable
A hashtable is an array that holds keys key-value pairs (where the item is of type void, meaning any type). The keys for each item are unique and implemented using a set. The hashtable data structure was utilized by the indexer to map each word found in the crawler produced directory in a set of counters with (documentID, count) pairs, where each documentID is a unique file name in the directory provided and count is the number of times the word was seen in the page. This index is then used by the querier to find the query and match it to the documents found in the index

#### Counters
A counter set is a set of counters, each distinguished by an integer key. It’s a set - each key can only occur once in the set - and it tracks a counter for each key. It starts empty. Each time counters_add is called on a given key, the corresponding counter is incremented. The indexer uses counters to map every word seen in the crawler produced directory to a counter set, where the keys are the file's document id and the count keeps track of how many times this word was seen in the file. Afterwards, the querier utilizes to solve the and/or sequences and keep track of their results

### Error handling 
- The querier will return error if any words cannot be normalized (for example those that have non-alphabetic characters)
- The querier will ignore and empty query
- The query will transform the query to lowercase to be able to find any word no matter in which format it's written in
- The query will ignore all spaces, so if a query has tabs, the words will still be able to be found
- The querier will stop if the direcotyr given is not produced by crawler by assuming that if the directory given contains a file `.crawler` file it is a crawler produced directory
- The querier will stop if the inde filename given is not readable
- The function will return a non-zero exit status upon any error initializing the data structures
- The querier will handle any wrongly formatted query and return an error message and prompt for another query

### Memory Allocation
1. The function `index_load` allocates memories for a hashtable structure, this is later on freed at the end of the main function
2. The `pair` structures allocate memory; these are later freed in the same function that they are allocated it, right after printing the results
3. The counters used to keep track of the and/or sequences use `counters_new()` which allocate memory. These are, therefore, then freed once the result is found and given to another counter called "result"
4. Once result is printed, this new counter is also freed
5. When the crawler file is read to find the url, memory is allocated to create the file name, this is later freed in the same function.
6. Another important allocation of memory is the query, the function `freadlinep` allocates memory, so once the EOF is found, then the query needs to be freed

### Resource Management
All the data structures utilized are found in the libraries `common.a` and `libcs50.a` which are found in the common/ and libcs50/ directories. Before running the querier function, these must be compiled by typing `make` in the terminal 