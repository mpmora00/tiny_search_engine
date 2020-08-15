# CS50 Lab 3
## Maria Paula, CS50 April 2020

### Crawler

A *crawler* is a module that crawls the web and retrives webpages beginning.

### Usage
The *crawler* module, implemented in `crawler.c`, exports the following function:

```c
/* crawl all pages reachable from seedURL, 
* following links to a maximum depth of maxDepth; 
* Returns:
*    non zero number if an error occured
*    0 if it crawled without any errors
*/
int crawler(char *seed_url, hashtable_t *hash_table, bag_t *bag, int max_depth, char *page_dir);

```
The crawler will crawl for a specific number of depths, starting at seedURL, until the maximum depth. In each depth, it will crawl all reachable pages available. A max depth of 0 means that only seedURL will be crawled. 

Once crawler has been compiled, it will be executed from the command line with the following syntax:

``` bash
$ ./crawler seedURL pageDirectory maxDepth
```

The arguments given to this command line are the following:
- seedURL: the URL where the crawling should begin. This URL must be internal and must be normalizable. 
- pageDirectory: must be an existing directory where the files of retrieved pages will be written with a unique ID name (starting with seedURL id 1)
- maxDepth: the maximum level of pages to explore

### Output
All errors are logged into standard error.
All files created will be written in the following format "pageDirectory/uniqueID", the crawler will assign the unique ids. The files will contain the page's url, depth, and HTML. 

### Exit Status:
0: succes, no errors <br/>
1: Wrong arguments/parameters given <br/>
2: Error creating modules necessary (bag/hashtable) <br/>
3: Error in the crawler function  <br/>

### Implementation

For more imformation, see [IMPLEMENTATION](IMPLEMENTATION.md)

We utilized a bag to keep track of all of the pages we need to crawl and a hastable to keeo track of all of the URLs that have been seen. 
The function works by continuing to occure while there are more webpages to crawl (meaning the bag is not empyty) and then processing each website to save into a file and scan all of the embedded links in the webpage. 

### Assumptions

No assumptions beyond those that are clear from the spec.

### Compilation

To begin the compilation, first make sure that `common.a` file is available in the directory `../common`. If it does not exist, move into the directory common and then write `make`. This should make a file called `common.a`. 

Afterwards, to compile, simply cd into the directory were all the hastable files are and write `make` in the terminal.

### Testing
To test, simply `make test`.
For this testing method, the file `testing.sh` is required since it provides the bash script that requires what will be run. 

See `testing.out` for a test that has already been run. 

See [TESTING](TESTING.md) for details of testing and an example test run.