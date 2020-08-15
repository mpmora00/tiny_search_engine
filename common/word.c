/* 
 * words.c 
 *
 * Maria Paula Mora, April 2020
 * Purpose: outputs a page to the appropriate file.
 */

#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/* Converts all letters in a word to lower case 
* ignore special characters and digits */
bool NormalizeWord(char* word) {
    // for every character in a word
    for (char *cp = word; *cp != '\0'; cp++){ 

        // if it's an alphabetic character
        if (isalpha(*cp)) {
            // change the character to lowercase
            *cp = tolower(*cp);
        }
        // word contains non-alphabetic character
        else {
            return false; 
        }
    }
    return true; 
}