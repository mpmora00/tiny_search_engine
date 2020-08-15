/* 
 * crawler.c 
 * see REQUIREMENTS.md and DESIGN.md for more information.
 *
 * Maria Paula Mora, April 2020
 * Purpose: to crawl the web and retrive webpages beginning 
 * from seed URL.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"
#include "memory.h" 
#include "pagedir.h" 

/**************** initialize functions used ****************/
int crawler(char *seed_url, hashtable_t *hash_table, bag_t *bag, int max_depth, char *page_dir);

int main(int argc, char *argv[]) {
    
    // if the wrong number of parameters is given
    if (argc != 4) {
        fprintf(stderr, "Error: Incorrect number of parameters given.\n");
        return 1; 
    }

    // parses arguments
    char *seed_url = argv[1];                       // root webpage
    char *page_dir = argv[2];                       // where to write downloaded pages
    int max_depth;                                  // maximum crawl depth
    int ret = sscanf(argv[3], "%d", &max_depth); 

    // check validity of arguments
    
    // 1. checks to see if pageDirectory exists
    if (!directory_exists(page_dir)) {
        fprintf(stderr, "Error: Directory %s does not exist\n", page_dir);
        return 1; 
    } 

    // 2. checks to see if URL is an internal URL and can be normalized
    else if (!NormalizeURL(seed_url) || !IsInternalURL(seed_url)) {
        fprintf(stderr, "Error: Incorrect URL given.\n");
        return 1;
    }

    // 3. check if an interger can be read and if it's not a non negative integer
   else if (ret != 1 || max_depth < 0) {
        fprintf(stderr, "Error: Incorrect Max Depth given.\n");
        return 1;
    }
    // set a maximal allowable value for maxDepth 
   else if (max_depth > 200) {
        fprintf(stderr, "Please insert a lower Max Depth value.\n");
        return 1;
    }
        
    //create a bag 
    bag_t *bag = bag_new();
    if (bag == NULL) {              // error creating a bag
        fprintf(stderr, "Error creating bag\n");
        return 2;
    }

    // create a hashtable
    hashtable_t *hash_table = hashtable_new(200);
    if (hash_table == NULL) {
        fprintf(stderr, "Error creating a hashtable\n");
        return 2;
    }

    // initializes other modules
    ret = crawler(seed_url, hash_table, bag, max_depth, page_dir);
   
    // free up space
    bag_delete(bag, NULL);
    hashtable_delete(hash_table, NULL);

    if (ret != 0) {
        // if crawler returned an error
        return 3; 
    }

    return 0; 
}


/* Uses a bag to track pages to explore, and hashtable to track pages seen; 
* when it explores a page it gives the page URL to the pagefetcher, 
* then the result to pagesaver, then to the pagescanner 
*
* Purpose: crawls the web and retrieves webpages starting from a “seed” URL */
int crawler(char *seed_url, hashtable_t *hash_table, bag_t *bag, int max_depth, char *page_dir){

    /* 
    * Used assert because on NULL return from malloc
    * it prints an error and exits with non-zero exit status.
    */
    char *url = count_malloc_assert(strlen(seed_url)+1, "Error allocating memory"); // root webpage
    strcpy(url, seed_url);
    // make a webpage for the seedURL, marked with depth 
    webpage_t *seed_page = webpage_new(url, 0, NULL); 

    // there was an error creation a website
    if (seed_page == NULL) {
        fprintf(stderr, "Error creating a website\n");
        return 1; 
    }

    // insert the seed to webpages to crawl
    bag_insert(bag, seed_page);

    // add that URL to the hashtable of URLs seen
    hashtable_insert(hash_table, seed_url, "");

    // initialize page ID and page being extracted 
    int page_id = 1; 
    webpage_t *curr_page;

    // while the bag is not empty meaning there are more webpages to crawl
    while ((curr_page = bag_extract(bag))!= NULL) {
        /* 
        * fetch HTLM contents of the page
        * pause for at least one second is done by pagefetcher (webpage_fetch) 
        */

        if(!webpage_fetch(curr_page)) {         // error fetching HTLM

            // log the error and keep going with other pages
            fprintf(stderr, "Could not fetch %s's HTML \n" , webpage_getURL(curr_page));
        } 
        
        // if we could fetch the URL
        else {

            // outputs a page to the appropriate file
            pagesaver(curr_page, page_dir, page_id);

            // increments ID by 1 for each new page 
            page_id += 1; 

            // if the webpage depth is < maxDepth, explore the webpage to find links
            if (webpage_getDepth(curr_page) < max_depth) {

                int pos = 0; 
                char *inside_url; 

                // Print “progress” indicators from the crawler as it proceeds
                fprintf(stdout, "Exploring URL: %s\n", webpage_getURL(curr_page));

                // parse the webpage to extract all its embedded URLs
                while ((inside_url=webpage_getNextURL(curr_page, &pos)) != NULL) {
                    
                    // Print “progress” indicators from the crawler as it proceeds 
                    fprintf(stdout, "Found URL: %s\n", inside_url);

                    // normalize the URL                      
                    if (!NormalizeURL(inside_url)) {
                        free(inside_url); 
                        fprintf(stderr, "Page can't be normalized.\n");
                    }
  
                    /* 
                    * if that URL is not ‘internal ignore it
                    * try to insert an internal URL into the hashtable of URLs seen 
                    * if hastable_insert is true == it was added to the table 
                    */
                    else if (IsInternalURL(inside_url) && hashtable_insert(hash_table, inside_url, "")) {
                        // make a new webpage for that URL, at depth+1
                        webpage_t *page_inside = webpage_new(inside_url, (webpage_getDepth(curr_page) + 1), NULL);   
                        
                        // there was an error creation a website
                        if (page_inside == NULL) {
                            fprintf(stderr, "Error creating website for page %s\n", inside_url);
                        }
                        else {

                            // Print “progress” indicators from the crawler as it proceeds
                            fprintf(stdout, "Added URL to bag: %s\n", inside_url);

                            // add the new webpage to the bag of webpages to be crawled
                            bag_insert(bag, page_inside);
                        }
                    }
                    else {
                        free(inside_url); 
                    }
                } 
            } 
        } 
        // free website space
        webpage_delete(curr_page);
    }
    
    return 0;
}