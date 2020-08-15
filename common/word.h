/* 
 * word.h - header file for CS50 pagedir module
 *
 * Purpose: converting all letters in a word to lower case
 *
 * Maria Paula Mora, May 2020
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>

/**************** NormalizeWord ****************/
/* Converting all letters in a word to lower case
 * The word given will be changed internally
 *
 * Caller provides:
 *   a word that contains 
 * 
 * We return: 
 *      true if the string was normalized correctly
 *      false if the string contains non-alphabetic character
 */
char* NormalizeWord(char* word); 

#endif // ____WORD_H