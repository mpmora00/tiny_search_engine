---
written by: Maria Paula Mora
layout: page
title: TSE Querier Design Spec
---

Recall the [Querier Requirements Spec](REQUIREMENTS.md); the **querier** reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin.

### User interface

The querier's interacts with the user in two ways, first the command line:

```
./querier pageDirectory indexFilename
```

Note: Do not assume the pageDirectory is entered with slash at the end. Hence, it could be entered as `./data/` or `./data` both of which are equivalent and should be handled correctly.

After the command line is input, the program with prompt the user for queries, these are and or sequences like the ones described in [README](README.md)

### Inputs and outputs

Input: besides the command line, the queries are the only inputs for this program
- These must be and/or sequences
- They cannot have any special characters or numbers 
- They can be capitalized and the words will be converted into lower case
- They can have multiple spaces and these will be ignored
- They cannot start or end with an "and" or and "or"
- An empty query is ignored

Output: we output the set of documents that matched the query in decreasing rank order in the following format: Score (ordered by size in decreasing order) docID (the document in the crawler directory): url (the url found inside that document or page). An example output when running the code onf the wikipedia playground in depth 1 is found below:

``` c 
// with a query of "computer and science" this is what the program will print
Query? computer and science
Query: computer and science 
Matches 6 documents (ranked):
score 170 doc   7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Computer_science.html
score   5 doc   4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
score   4 doc   2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Linked_list.html
score   4 doc   3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Hash_table.html
score   1 doc   1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
score   1 doc   6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
```

### Functional decomposition into modules

We anticipate the following modules or functions:

querier.c
1. *main*, which parses arguments and initializes other modules, initialize data structure index
2. *divide_string(array, string, number of words in string)* which breaks a single string into an array of strings for easy parsing of the query
3. *word_counter(string)* to calculate the number of words in a string (so that it can allocate the correct number of memory in the array mentioned above)
4. *getURL(pageDirectory, docID) * which goes into the docID and obtains the first line of the file or the url
5. *print_results(resulting counters, pageDirectory)* which prints the results in the format mentioned above
6. *parse_query(array, number of words in string)* which cleans and parses each query according to the syntax described below.
7. *satisfy_query(index, resulting counter, array of query, number of words in string)* which identifies the set of documents that satisfy the query

Other important functions are all of the functions that will be required for `counters_iterate()` and other functions found in `common.a` and `libcs50.a`
 
Some helper modules that provide data structures:
 1. *hashtable* filled with sets of counters to keep track of the index
 2. *set* maps from a word to a list of (docID, count) pairs
 3. *counter* keeps track of how many times a word was seen in a specific docID

### Pseudo code for logic/algorithmic flow
The Querier will run as follows:
1. Execute from a command line as shown in the User Interface
2. Process and validate command-line parameters
3. Initialize data structure index
4. index_load(filename, index)
5. promt for a query
6. read from the standard input until EOF
    7. clean and parse each query
    8. translate all upper-case letters on the input line into lower-case (check that there aren't any special characters or digits)
        9. if the query syntax is somehow invalid, print an error message, do not perform the query, and prompt for the next query
        10. else print the ‘clean’ query
    11. check if the query follows the correct syntax 
        12. if the query is empty (no words), print nothing
    13. for every word in the query
        14. while the word is not an "or"
            15. add the first word into res_and
            16. if the word is not an "and"
                17. add the word into res_and (calculating the lowest score)
        18. move res_and into res_or and add the scores
        19. return res_or
20. if no documents satisfy the query, print "No documents match"
21. print the set of documents in decreasing rank order
22. Exit with zero status when EOF is reached on stdin.

The querier completes and exits when the user declares it the end of the stdin.

### Dataflow through modules

querier.c
 1. *main*, parses parameters (uses ``isCrawlerDirectory``) and  initialize data structure index, passes them to the ``index_build``
 2. *index_load(file1, index)*, reads an index based file and creates a hashtable index 
 3. *word_counter* (string)* to calculate the number of words in a string (and allocate the correct number of memory in an array)
 4. *divide_string(array, query, count)* to divide the query into separate words
 5. *parse_query(words, count)* to clean and parse each query according to the syntax described above
 6. if correct *satisfy_query(index, a new counter, array, size of array)
 7. *print_results(counter of result, pageDirectory)* to print out the results
 7. *index_delete* once the program has ended, clean up by freeing hashtable and constituent data structures

### Major data structures

Three helper modules provide data structures:

 1. *hashtable* of sets with words as keys and counters as items
 2. *counters* of document names. Count is how many times a word was seen in a specific document
3. *sets* which will map from words to counters of (docID, count)

### Testing plan

*Unit testing*.  A small test program in `testing.sh` to test each module to make sure it does what it's supposed to do.

*Integration testing*.  Assemble the querier and test it as a whole using `testing.sh`.
In each case, examine the output files carefully to be sure they have the correct representation of the querier data structure. 

### Indexer.c Testing
#### Incorrect Inputs
- Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.
Test the querier with a `pageDirectory` that points to a non-existing directory
Test the querier with a `pageDirectory` that points to non-crawler-created directory  
Test the querier with an invalid filename given as a second parameter
More than 3 parameters
Less than 3 parameters
Test the query with bad characters like special characters and digits
Test the query with an "and" at the beginning
Test the query with an "and" at the end
Test the query with an "or" at the beginning
Test the query with an "or" at the end
Test the query with an "and" and "or" adjacent. 

#### Correct Inputs
- Test the querier with a `pageDirectory` in different formats (./directory, /directory,directory/)
- Test the querier with a correct `indexFilename` and `pageDirectory` with different queries
    - Queries that should not find a match

    - Capitalized queries
    - Queries with weird spacing
    - Test a query without the words "and" or "or"
    - Test a single word query
    - Test and empty query 

    - Queries that should find a match
    - Only andsequences
    - Only orsequences
    - A mixture of and and or sequences (intertwined)