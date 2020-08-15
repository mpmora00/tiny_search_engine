# CS50 Lab 3
## Maria Paula, CS50 May 2020

### Query

An *query* is a program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin.

### Usage
`querier.c.c` only contains one global function, which is the main, the others are local. However, it utilizies a lot of functions from `pair.h`, `pair.c`, `iterate.h`, and `iterate.c` as helpers. Thesw ill be explained below
 simply contain a main function. However, they obtain functions from several other libraries like `libscs50-give.a` and `common.a`.

#### Functionalities
The functionalities I implemented in this program are the follow:
- The querier prints the set of documents that contain all the words in the query
- The querier also supports ‘and’ and ‘or’ operators, but without precedence (in mathematical terms, it treats them as left associative, equal precedence operators).
- The querier also supports ‘and’ precedence over ‘or’.
- The querier also prints the document set in decreasing order by score.

#### Querier description
`querier.c` execute from a command line with usage syntax:
``` bash
./querier pageDirectory indexFilename
```
- where pageDirectory is the pathname of a directory produced by the Crawler
- where indexFilename is the pathname of a file produced by the Indexer

Once it's called, the program will:
1. load the index from indexFilename into an internal data structure
2. read search queries from stdin, one per line, until EOF (Control+D)
3. clean and parse each query according to a syntax described below
    - if the query syntax is invalid, it will print an error message and ignore this query 
        - then it will prompt for the next query.
    - if it works, it will print the ‘clean’ query
4. use the index to identify the set of documents that satisfy the query
5. if the correct query is given, the program will rank the resulting set of documents according to its score
6. print result
7. exit with zero status when EOF is reached on stdin.

##### Output:
- if the query is empty, it will print nothing
- if no documents satisfy the query, prints "No documents match."
- otherwise, it will print the set of documents in decreasing rank order
    - for each, it lists the score, document ID and URL

##### Standard Input 
The querier prompts the user for queries, which are comprised of words, with optional and/or operators. For example,
``` 
computer science
computer and science
computer or science
baseball or basketball or ultimate frisbee
```
The first two examples are treated identically, matching only documents that have both words - not necessarily together. The third picks up documents that have either word and the fourth matches documents that mention baseball, or basketball, or both “ultimate” and the word “frisbee” (not necessarily together).

Some forms of incorrect input are the following:
``` c
// bad character '-' in query.
non-letters are not allowed

// bad character '5' in query.
how about digits like cs50

// 'and' cannot be first
and is not a valid query

// 'or' cannot be first
or cannot be searched 
```

After you have finished the query and want to begin searching, use enter. 
If you want to finish the entire program, use Control + D. 

#### Pair description
The documents `pair.h` and `pair.c` defines a structure called pair which holds two items, `int docID` and `int score`. This structure is utilized to sort the resulting set of documents in order of score value. This makes it easier to pass into an array than a counter pointer which is what we had before. 

The functions `score_get()` and `docID_get` which are defined in this file, simplify the process of sorting, finding the URL, and printing these values greatly.

This structures functions are described in more depth in [[IMPLEMENTATION.md](IMPLEMENTATION.md)

#### Iterate description
The documents `iterate.h` and `iterate.c` is a series of functions that can be utilized inside the function `counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count))` as itemfun. Every function interacts with the counters in a different way and edits them accordingly. This file's functions are used throughout querier for different parts of the program.

These iteration functions are described in more depth in [[IMPLEMENTATION.md](IMPLEMENTATION.md)

### Output
All errors are logged into standard error.

### Exit Status (for querier)
0: Success, no errors <br/>
1: Wrong arguments/parameters given <br/>
2: Error creating modules necessary (hashtable) <br/>
3: Error build or saving index  <br/>

### Implementation

For more imformation, see [IMPLEMENTATION](IMPLEMENTATION.md)

Our Querier loads the index and then prompts the user for queries. Queries are comprised of words, with optional and/or operators like where mentioned above. 

The function uses a hashtable of sets of counters to keep track of the number of occurences of a specific word in a document. The hashtable maps from (words, counters) and the counters map from (documets, number of ocurrences). It also finds a resulting counter to keep track of the the matched pairs from the index. Finally, it uses pairs to sort the matches acording to score easily. 

### Assumptions
`querier.c` assume that the input directory and files follow the designated formats.
- pageDirectory has files named 1, 2, 3, …, without gaps.
- The content of files in pageDirectory follow the crawler format defined in [README](../crawler/README.md)
- The content of the file named by indexFilename follows our Index file format

### Compilation

To begin the compilation, first make sure that `common.a` file is available in the directory `../common`. If it does not exist, move into the directory common and then write `make`. This should make a file called `common.a`. 

Moreover, make sure that `libcs50-give.a` file is available in the directory `../libcs50`.  

Afterwards, to compile, simply cd `../indexer`  and write `make` in the terminal.

### Testing
To test, simply `make test`.
For this testing method, the file `testing.sh` is required since it provides the bash script that requires what will be run. 

See `testing.out` for a test that has already been run. 

See [TESTING](TESTING.md) for details of testing and an example test run.