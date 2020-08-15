# CS50 Lab 3
## Maria Paula, CS50 May 2020

### Testing 'indexer'

To test my *indexer* module I wrote a test program, [testing](testing.sh), to explore the capabilities of the *indexer* data structure and visualize the output. 

### Test file explanation
`testing.sh` was written to be able to see the progress of both `indexer.c` and `indextest.c` on different crawler-made directories. In order to throughout check the code, I split this test in three parts:

1. Testing incorrect parameters or commands that should fail (for both `indexer.c` and `indextest.c`): 
	- Here I tested what happened if less or more than 4 arguments are given 
	- Moreover, I tested what happens if you give any incorrect parameters (nonexistent directory, invalid filename, noncrawler directory, nonexistent oldIndexFilename given)

2. Testing correct inputs or commands that should succeed for `indexer.c`
	- First, I tested what would happen if `indexer.c` is given a file that already exists
    - Then, I tested what happened if I input different pageDirectory formats, for example entered with slash at the end. If given the data directory I wanted to check if the parameters `./data` and `./data/` would work. 
    - Afterwards, I tested 4 different directories with different amount of files to see how the indexer reacted to each

3. Testing correct inputs or commands that should succeed for `indextest.c`
    - After `indexer.c` was tested and indexer files were created we can test the `indexer.c` methods
        - Here, I tested two different files on `indexer.c`
        - Then utilized `indexsort.awk` to sort both the indexer produce files and the indextester files. 
        - Once they where sorted, I used the `diff` functions to find any differences between the files
            - If the `diff` function had an exit code of 0, then I print that there are no differences in the files, else we print that there where differences between the files


### Test run
To test, move into the indexer directory write `make`. After the indexer and indextest executable files are made, then you can run the test options. 

An example test run is in `testing.out`, built with

	make test &> testing.out

In that file one can see the results of all of the explanations mentioned above. 

### Limitations
For `indexer.c`, the hashtables created has a fixed size of 900, this means that if a large website is explored with a lot of different words, the hashtable will reach a limit and will not be efficient.