# CS50 Lab 3 Implementation of crawler
## Maria Paula Mora, CS50 May 2020

### Pseudocode for each object/components/functions 

Some functions that both Indexer and Indextest utilize are the following:
#### Index_build
1. For each file in (pageDirectory)
    2. Convert the document ID to a webpage
		create counters 
	3. For every word in the file with length >= 3 and with alphabetic characters only
		4. Normalize the word (convert to lowercase)
	    5. If the word is not in the hashtable yet
			6. add word to hashtable with a counter for this specific file id starting at 1
		7. If hashtable does contain word
			8. If counters of this word contain the docID
				9. Increment the count
			10. If this docID does not exist 
				11. Add counter to the hashtable with (docID, 1)
		
#### Index_save
1. Loop through the hashtable
	2. In the file given, on each line, write the word followed by it's documentID and count following this format:
        * one line per word, one word per line
        * each line provides the word and one or more (docID, count) pairs, in the format *word docID count [docID count]...*
        * where word is a string of lower-case letters,
        * where docID is a positive non-zero integer,
        * where count is a positive non-zero integer,
        * where the word and integers are separated by spaces.
        * Within the file, the lines may be in any order.
        * Within a line, the docIDs may be in any order.

#### Index_load
1. For every line in a file
    2. Read the first word of the file 
    3. Read (%d %d) pairs
        4. Insert into a counter
    5. Insert the word and the counters into the hashtable

#### Index_delete
1. For every slot in the hashtable
    2. free the counter, the word, and the set
3. Free the hashtable memory

#### Indexer
1. Execute from a command line
2. Process and validate command-line parameters
3. Initialize a hashtable data structure data structure which will be used to create the inverted index
4. Call `index_build(directory, index)`
5. Call `index_save(file, index)`
6. Call `index_delete(index)`

#### Indextest
1. Execute from command line 
2. Process and validate command-line parameters
3. Initialize a hashtable data structure data structure which will be used to create the inverted index
4. Call `index_load(file1, index)`
5. Call `index_save(file2, index)`
6. Call `index_delete(index)`

In summary, the job of the indexer is to read the documents in the pageDirectory output by the crawler, build an inverted index mapping from words to documents, and write that index to a file. The job of the indextest is to load the index from the oldIndexFilename into an inverted-index data structure and save this into a file (can be used to test the indexer module).

### Function Parameters
`indexer.c` and `indextest.c` run from the command line with the following syntax:
``` bash
./indexer pageDirectory indexFilename
./indextest oldIndexFilename newIndexFilename
```
- For `indexer.c`, the pageDirectory is the pathname of a directory produced by the Crawler and the indexFilename is the pathname of a file into which the index should be written. 
- For `indextest.c`, the pageDirectory is the pathname for an existing directory produced by the crawler and indexFilename is the pathname of a writeable file; it may or may not already exist.

Since both of these functions utilized functions from the ``common.a`` lib for its implementations, then the parameters are very important to mention. You can see common's [../common/README.md](README.md) for more in-depth definitions of these functions.

``` c
/* Creates an index from a directory 
 * 
 * Parameters:
 *   A crawler produced directory to create index from
 *   A Hashtable pointer to build the index in
 * 
 * * Returns:
 *   1 if there was any error
 *   0 if the function exited correctly
 */
int index_build(char *page_dir, hashtable_t *hash_table);

/* Saves an index into a file
 *
 * Parameters:
 *   A name of a file to save the index in
 *   A hashtable pointer where index was built
 * 
 * Returns:
 *   1 if there was any error
 *   0 if the function exited correctly
 */
int index_save(char *filename, hashtable_t* hash_table);

/* Free's up all of the memory allocated by the index
 * 
 * Parameters:
 *   A hashtable  where index was built
 */
void index_delete(hashtable_t* hash_table);

/* Load an index from a file into an inverted-index data structure
 * 
 * Parameters:
 *   A Hashtable pointer to build the index in
 *   A indexer produced directory to create index from
 * 
 * Returns:
 *      1 if an error was encountered
 *      0 if the program exited correctly 
 */
int index_load(FILE *fp_in, hashtable_t* hash_table);
```

### Data structures utilized 
These definitons come from the Lab 3 webpage (https://www.cs.dartmouth.edu/~cs50/Labs/Lab3/)

#### Hashtable
A hashtable is an array that holds keys key-value pairs (where the item is of type void, meaning any type). The keys for each item are unique and implemented using a set. The hashtable data structure was utilized by the indexer to map each word found in the crawler produced directory in a set of counters with (documentID, count) pairs, where each documentID is a unique file name in the directory provided and count is the number of times the word was seen in the page.

#### Counters
A counter set is a set of counters, each distinguished by an integer key. It’s a set - each key can only occur once in the set - and it tracks a counter for each key. It starts empty. Each time counters_add is called on a given key, the corresponding counter is incremented. The indexer uses counters to map every word seen in the crawler produced directory to a counter set, where the keys are the file's document id and the count keeps track of how many times this word was seen in the file. 

### Error handling 
- The indexer will skip over words that can't be normalized for example those that have non-alphabetic characters)
- The indexer will avoid reading over directories that are not produced by crawler by assuming that if the directory given contains a file `.crawler` file it is a crawler produced directory
- Both `indexer.c` and `indextest.c` will return a non-zero exit status upon any error initializing the data structures
- If `indexer.c` reads over a file in the directory that does not have the correct format to create a webpage (url, depth, html) it will stop executing.
    - Similarly for `indextest.c` and a file with the format (word integer integer...)
- Since `indexer.c` assumes that the files inside crawler produced directory are named 1, 2, 3... without gaps, if there is a gap then the indexer stops executing, believing that all files have been checked. 

### Memory Allocation
1. In both [indexer](indexer.c) and [indextest](indextest.c), `hashtable_new()` and `counters_new()` allocate memory for these structure. Therefore this are freed at the end of the functions. 
3. When new files are created, memory is allocated to create the file name, this is later freed in the same function.

### Resource Management
All the data structures utilized are found in the libraries `common.a` and `libcs50.a` which are found in the common/ and libcs50/ directories. Before running the indexer and indextest functions, these must be compiled by typing `make` in the terminal 