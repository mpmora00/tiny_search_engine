# CS50 Lab 3
## Maria Paula, CS50 May 2020

### Testing 'querier'

To test my *querier* module I wrote a test program, [testing](testing.sh), to explore the capabilities of the *querier* data structure and visualize the output. 

### Test file explanation
`testing.sh` was written to be able to see the progress of `querier.c` on different queries of the same index and crawler file. In order to cover all basis, I split this test in three parts:

1. Testing incorrect parameters or commands that should fail: 
	- Here I tested what happened if less or more than 4 arguments are given 
	- Moreover, I tested what happens if you give any incorrect parameters (nonexistent directory, noncrawler directory)

2. Testing correct inputs or commands that should succeed for `querier.c`
    - Then, I tested what happened if I input different pageDirectory formats, for example entered with slash at the end. If given the data directory I wanted to check if the parameters `./data` and `./data/` would work. 
    - Afterwards, I tested several types of queries
        - Queries that should fail becuase of special characters or incorrect syntax
        - Queries that should find no matches
        - Queries that should find matches
        - Different spacing and capitalization

3. Used fuzzquery to generate random inputs and test my code on that

### Test run
To test, move into the querier directory write `make all` (not make because `fuzzquery` is nedded to be able to run the test). After the querier and fuzzquerier executable files are made, then you can run the test options. 

An example test run is in `testing.out`, built with

	make test &> testing.out

In that file one can see the results of all of the explanations mentioned above. 

### Limitations
For `querier.c`, the code assumes that the file name given is correct and that it has the correct input. Therefore, if an incorrect filename is given, then the query will still assume that it's correct and proceed.