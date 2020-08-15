# CS50 Lab 3
## Maria Paula, CS50 April 2020

This module support the TSE project.

### Common

The common directory is a library of code, that will eventually be shared by the crawler, indexer, and querier.
It contains 6 files: `Pagedir.h`, `Pagedir.c`, `index.c`, `index.h`, `word.h`and `word.c` and several important functions which will be explained below

### Usage
To build `common.a`, run `make`. 
To clean up, run `make clean`.T

### File Overview

1. The ``Pagedir`` module is defined in `Pagedir.h` and implemented in `Pagedir.c` and it has the following functions:

```c
/* Outputs a page to the appropriate file.
 * Return:
 *   0 if the file was written correctly.
 *   1 if the file was not opened correctly
 */
int pagesaver(webpage_t *page, char *page_dir, int current_id);

/* Check if a directory is existent.
* Return:
*   returns true if the directory exists
*   false if it does not exist 
*/
bool directory_exists(char *name_dir); 

/* Verifies whether dir is indeed a Crawler-produced directory.
 *
 * Returns:
 *   true if the dir is indeed a Crawler-produced directory
 *   false if dir is not a Crawler-produced directory
 */
bool is_crawler_directory(char *dir);

/* Verifies whether the file name is a valid pathname
 *
 * Returns:
 *   true if the file name is a valid pathname
 *   false if file name is not a valid pathname
 */
bool valid_filename(char *index_filename);

/* Create a wepage from a file
 *
 * Returns:
 *   NULL if there was an error opening/reading the file or saving the website
 *   webpage pointer if everything was done correctly
 * 
 * Caller must remember to free the memory for the webside
 */
webpage_t* page_load(char *page_dir, char *filename);
```

2. The ``word`` module is defined in `word.h` and implemented in `word.c` and it has the following functions:

```c
/* Converting all letters in a word to lower case
 *
 * Returns:
 *   NULL if there was any error
 *   A lowercase string if no errors where encountered
 *
 */
char* NormalizeWord(char* word); 
```

3. The ``index`` module is defined in `index.h` and implemented in `index.c` and it has the following functions:

```c
/* Creates an index from a directory
 *
 * Returns:
 *   1 if there was any error
 *   0 if the function exited correctly
 * 
 * The caller is later responsible of freeing this hashtable outside the function
 */
int index_build(char *page_dir, hashtable_t* hash_table);

/* Saves an index into a file
 *
 * Returns:
 *   1 if there was any error
 *   0 if the function exited correctly
 */
int index_save(char *filename, hashtable_t* hash_table);

/* Free's up all of the memory allocated by the index
 */
void index_delete(hashtable_t* hash_table);

/**************** index_load ****************/
/* Load an index from a file into an inverted-index data structure
 *
 * Returns:
 *      1 if an error was encountered
 *      0 if the program exited correctly 
 */
int index_load(FILE *fp_in, hashtable_t* hash_table);
```

#### Saved file format 

##### Pagesave
The file will be saved with the name "page_dir/ID" meaning that it will be saved inside the given page directory with the name of the current ID. The file will have the following format:
1. First line will be the page URL on the first line.
2. Second line will be the depth of the page (where the seed is depth 0).
3. Third line will be the page contents.

##### IndexSaver
The file will be saved with the name "filename" meaning that it will be saved under whatever parameter filename was given. The file will have the following format:

1. one line per word
2. each line provides the word and one or more (docID, count) pairs, in the format "word docID count [docID count]…"
    - where word is a string of lower-case letters,
    - where docID is a positive non-zero integer,
    - where count is a positive non-zero integer,
    - where the word and integers are separated by spaces.
    - Within the file, the lines may be in any order.
    - Within a line, the docIDs may be in any order.

#### Implementation
Here we will explain some of the functions implemented

##### Directory_exists
In order to check if the directory exists in the function `directory_exists`, a new file is attempted to be written in that directory. If the file is not able to be opened, it means that the directory does not exist. 

After this function is function finished running, a new file called `.crawler` will be have made in the directory. 

##### page_load
- From the directory and the filename provided, allocates the memory to create a filename with the format "directory/filename"
- Opens the file
- Reads the first and second line and saves then as URL and depth correspondigly
- Reads the rest of the file and saves it as the HTML 
- Creates a website from the strings read
- Frees all of the allocated memory and closes the file

##### index_build
- Loops through every document in a directory and creates a webpage
- Loops through every word in that webpage and adds it to the index accordingly (only words with 3 or more characters)
- Returns an index (or a hashtable of counters)

### Assumptions

We are assuiming the the user has provided for the `pagesaver` function:
- A valid webpage with an HTML inside it 
- A valid page directory that exists 
- A positivie current ID

Moreover, the user must delete/free the webpage space outside of this function

For `page_load` the caller must free the webpage memory outside of the function

For `index` the caller must free the hashtable memory outside of the function

The functions takes care of free any memory it allocated, so no assumptions beyond the ones mentioned above
