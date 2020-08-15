# CS50 Lab 3
## Maria Paula, CS50 May 2020

### Index

An *index* is a module that builds an inverted-index data structure mapping from words to (documentID, count) pairs, wherein each count represents the number of occurrences of the given word in the given webpage.

### Usage
Both `indexer.c` and `indextest.c`, simply contain a main function. However, they obtain functions from several other libraries like `libscs50-give.a` and `common.a`.

#### Indexer
`indexer.c` will read every single file in a crawler-created directory and map every single word (that has three or more characters) into a hashtable This hashtable is made of a set of counters which maps from words to (documentID, count). 
    - Afterwards, this function saves this indexer into a file in the format:
        - one line per word, one word per line
        - each line provides the word and one or more (docID, count) pairs, in the format "word docID count [docID count]…"
        - where word is a string of lower-case letters,
        - where docID is a positive non-zero integer,
        - where count is a positive non-zero integer,
        - where the word and integers are separated by spaces.
        - Within the file, the lines may be in any order.
        - Within a line, the docIDs may be in any order.
Once the document is saved, all of the memory allocated for the hashtable is freed and deleted

Once indexer has been compiled, it will be executed from the command line with the following syntax:

``` bash
$ ./indexer pageDirectory indexFilename
```

The arguments given to this command line are the following:
- pageDirectory: is the pathname of a directory produced by the Crawler
- indexFilename: is the pathname of a file into which the index should be written
    - The indexer creates the file (if needed) and overwrites the file (if it already exists)

#### Indextest
`indextest.c` will reade a given file (which is a saved index) and recreate an index from that file. This new, recreated index, will then be saved into another file so that these two can be compare. Once the document is saved, all of the memory allocated for the hashtable is freed and deleted

Once indexer has been compiled, it will be executed from the command line with the following syntax:

``` bash
$ ./indextest oldIndexFilename newIndexFilename
```

The arguments given to this command line are the following:
- oldIndexFilename: is the name of a file produced by the indexer
- newIndexFilename: is the name of a file into which the index should be written
    - The indexer creates the file (if needed) and overwrites the file (if it already exists)

### Output
All errors are logged into standard error.

### Exit Status:
#### Indexer
0: Success, no errors <br/>
1: Wrong arguments/parameters given <br/>
2: Error creating modules necessary (hashtable) <br/>
3: Error build or saving index  <br/>

#### Indextest
0: Success, no errors <br/>
1: Wrong arguments/parameters given <br/>
2: Error creating modules necessary (hashtable) <br/>
3: Error loading or saving index  <br/>

### Implementation

For more imformation, see [IMPLEMENTATION](IMPLEMENTATION.md)

The function uses a hashtable of sets of counters to keep track of the number of occurences of a specific word in a document. The hashtable maps from (words, counters) and the counters map from (documets, number of ocurrences). 

### Assumptions
`indexer.c` assumes that:
- pageDirectory has files named 1, 2, 3, …, without gaps.
- The content of files in pageDirectory follow the format as defined in the specs
- The indexer assumes that if the directory given contains a file `.crawler` file it is a crawler produced directory

`indextest.c` assumes that the content of the given file follows the format specified above.


### Compilation

To begin the compilation, first make sure that `common.a` file is available in the directory `../common`. If it does not exist, move into the directory common and then write `make`. This should make a file called `common.a`. 

Moreover, make sure that `libcs50-give.a` file is available in the directory `../libcs50`.  

Afterwards, to compile, simply cd `../indexer`  and write `make` in the terminal.

### Testing
To test, simply `make test`.
For this testing method, the file `testing.sh` is required since it provides the bash script that requires what will be run. 
- This file also makes sure that crawler files have been created first

See `testing.out` for a test that has already been run. 

See [TESTING](TESTING.md) for details of testing and an example test run.