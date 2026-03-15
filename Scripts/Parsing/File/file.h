/**
 *  @file   file.h
 *  @brief  Converts files to strings and gets file information.
 *  @author Joel Nelems
 *  @date   2026-3-9
 *
 *  @details
 *  The file_to_string() function takes a file opened in read mode and converts it into a dynamically allocated
 *  string. This allows the file text to be used with the lexer struct that requires a string.
 *
 *  @example
 *  size_t numChars;
 *  FILE  *fp     = fopen("example.txt", "r");
 *  char  *script = file_to_string(fp, &numChars);
 */

// Include guards
#ifndef FILE_H
#define FILE_H

// Include libraries
#include <stdio.h>

// Declare functions
unsigned char *file_to_string(FILE *file, size_t *numChars);

// Include guard
#endif //FILE_H
