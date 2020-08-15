# CS50 Lab 3
## Maria Paula, CS50 April 2020

### Testing 'crawler'

To test my *crawler* module I wrote a test program, [testing](testing.sh), to explore the capabilities of the *crawler* data structure and visualize the output. 

### Test file explanation
`testing.sh` was written to be able to se progress of crawling different seedURLs with different depths. In order to throughout check the code, I split this test in two parts: 

1. Testing incorrect parameters or commands that should fail
	- Here I tested what happened if less or more than 4 arguments are given 
	- Moreover, I tested what happens if you give any incorrect parameters (externalURL, negative maxdepth, string for maxdepth, unexistent directory)
	- Finally, I tested what happens if you give a non-internal server or a valid server but non-existent page.

2. Testing correct inputs or commands that should succeed
	- Here I tested 4 seedURLs with different depths to test what results they give and if different depths are correctly supported
	- Moreover, I tested what happened if I input different pageDirectory formats, for example entered with slash at the end. If given the data directory i wanted to check if the parameters `data`, `./data` and `./data/` would work. 

### Test run
To test, move into the crawler directory write `make`. After the crawler executable file was made, then you can run the test options. 

I tested the Wikipedia playground at depths 0, 1, 2. Fortunately, the code works perfectly with no errors thrown. However, it takes such a long time to run at depth 2 or higher! For this reason, it is in my `testing.sh` but it is commented out, if you want to run it, uncomment it.


An example test run is in `testing.out`, built with

	make test &> testing.out

In that file one can see the results of all of the explanations mentioned above. 

### Limitations
The hashtable is of fixed size of 200, this means that if more webpages are explored (meaning a large depth, then the code will not be efficient)
 
Because the hashtable has a fixed size of 200, I decided to set the maximal allowable value for maxDepth at 200 as well, to prevent an overload of information that can't fit in the hashtable. 