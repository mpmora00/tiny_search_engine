# CS50 Lab 3 Implementation of crawler
## Maria Paula, CS50 April 2020

### Pseudocode for each object/components/functions 

#### Crawler 
After checking if the given parameters are correct, the crawler runs in the following steps:
1. Make a webpage from the seedURL given as a parameter
2. Create a bag to keep track of any website that still needs to be crawled
3. Create a hashtable that keeps track of all of the visited URLS 
4. While there are still webpages to be crawled (or inside the bad)
	- Extract a webpage from the bad
    - Pause for a second and then fetch it's data (meaning retrieve the page's HTML)
    - Write the webpage to the pageDirectory given with a unique ID (first line of file is URL, second line is depth, third line is HTML content)
    - If this website's depth is less than the Max Depth given
        - Explore the website to find embedded links and for each of these
        - If the link can be normalzed is an internal URL and has not been inserted into the hashtable yet, then 
            - Add it to the hashtable 
            1- Make a new webpage for that URL with a depth of current depth + 1 
            - Add that page to the pag

In summary, this program "crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given depth".

##### Function Parameters
For the crawler function, there are five main parameters. Three of then, are provided by the user:
1. ```seed_url```: this should be a link to an internal website that can be normalized. 
2. ```page_dir```: this is the name of a directory that should exists and have the necessary permissions
3. ```max_depth```: this should be a non-negative integer that describes the number of depths that the caller wants the crawler want to explore from the seedURL

``` c
int crawler(char *seed_url, hashtable_t *hash_table, bag_t *bag, int max_depth, char *page_dir);
```

The other two parameters, are a bag and a hashtable which are created by the main function. 

#### Pagesaver (inside pagedir.c)
1. Create a new file with the correct format (pagedirectory/uniqueID)
2. Write the webpage's URL in the first line
3. Write the webpage's depth in the second line
4. Write the webpage's entire HTML startin in the third line

##### Function Parameters
For the pagesaver function, there are 3 main parameters:
1. ```page```: this is the current page we are describing in the file being saved
2. ```page_dir```: this is the name of a directory that should exists and have the necessary permissions
3. ```current_id```: this is a unique ID value that is calculated by crawler for each page that is saved

```c 
int pagesaver(webpage_t *page, char *page_dir, int current_id) {
```

### Data structures utilized 
These definitons come from the Lab 3 webpage (https://www.cs.dartmouth.edu/~cs50/Labs/Lab3/)

#### Bag
A bag is a linked list that holds a collection of items. As the user adds more items, the bag grows and as the user extracts, it shrinks. The bag data structure was utilized by crawler as a way to keep track of which websited we still need to visit and explore. For this, I utilized the `bag_extract()` function, which acts like a stack and pops out the items in no particular orded (this means that pages are not explored in a set order)

#### Hashtable
A hashtable is an array that holds keys key-value pairs (where the item is of type void, meaning any type). The keys for each item are unique and implemented using a set. The hashtable data structure was utilized by crawler as a way to keep track of which websites had been explored already and should not be explored again. Since `hashtable_insert(hash, key, item)` returns false if the key was already found, this function helps the crawler function to avoid exploring the same webpage twice. 

### Error handling 
- Upon any error in fetching a page's HTML, the error is logged and crawler continues to the next URL.
- Upon any error in writing the page to pageDirectory, the error is log and crawler continues.
- Upon any error in Normalizing URL, log the error and the page will be ignored (embedded links will not be explored).
- Upon finding any page that is not Internal, log the error and the page will be ignored (embedded links will not be explored).
- Upon any creating the bag or the hastable, the error is log and the entire crawler function stops.
- Upon NULL return from malloc/calloc, crawler prints an error and exits with non-zero exit status (using assert function).
- Upon any incorrect parameters given, the error is log and the entire crawler function stops.
- Upon encountering an error creating a webpage from the seedURL, the error is log and the entire crawler function stops.

### Memory Allocation
1. In [crawler](crawler.c), both `bag_new()` and `hashtable_new()` allocate memory for these structure. Therefore this are freed at the end of crawler. 
2. When crawler explores the embedded links , it utilizes `webpage_getNextURL`. This function allocates memory for the URL and is later freed by crawler using the function `webpage_delete()` 
3. When new files are created, memory is allocated to create the file name, this is later freed in the same function `pagesaver`
4. Since `webpage_delete()` frees memory for all page URLS, the crawler has to allocate memory for the `seedURl` as well to avoid encountering any errors

### Security and privacy properties
Since web servers do not like crawlers and may block it, then the crawler purposely slows its behavior by introducing a delay, sleeping for one second between fetches.<br/>
Moreover, the crawler also limits its crwals to a specific web server inside the Computer Science Deparment, so not other servers on campus are bothered to avoid blocking the whole of dartmouth.edu.<br/>
The crawler stays within the CS50 playground http://old-www.cs.dartmouth.edu/.

### Resource Management
All the data structures utilized are found in the libraries `common.a` and `libcs50.a` which are found in the common/ and libcs50/ directories. Before running the crawler function, these must be compiled by typing `make` in the terminal 