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

### Assumptions

We are assuiming the the user has provided for the `pagesaver` function:
- A valid webpage with an HTML inside it
- A valid page directory that exists
- A positivie current ID

Moreover, the user must delete/free the webpage space outside of this function

For `page_load` the caller must free the webpage memory outside of the function

For `index` the caller must free the hashtable memory outside of the function

The functions takes care of free any memory it allocated, so no assumptions beyond the ones mentioned above
