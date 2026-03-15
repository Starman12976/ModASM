/**
 *  @file   lexer.h
 *  @brief  Parses a ModASM file.
 *  @author Joel Nelems
 *  @date   2025-3-9
 *
 *  @details
 *  The lexer structure reads through a string char-by-char using the advance() function. It automatically updates the
 *  position data (offset, line, column) based on the character that is consumed. Most characters increment the column,
 *  newlines reset the column and increment the line count. It is recommended to allow the lexer to update its own
 *  values and not change them manually. Each lexer holds a pointer to its associated functions, allowing class-like
 *  methods. The lexer still has to be passed when calling them.
 *  The lexer has three primary functions:
 *  - peek(), which returns a character at the lexer's current offset.
 *  - advance(), which returns the current character and increments the offset.
 *  - get_lexeme(), which copies a string from the lexer's source text at a given position without updating the lexer's
 *  position values. These methods are used for the tokenization step of the assembler.
 *  Creating a lexer should be done through the lexer_init() function. This function assigns a given script to the lexer
 *  and populates the rest of the values automatically, including the function pointers.
 *
 *  @example
 *  Lexer* lexer = lexer_init(script);
 *  char c = lexer->peek(lexer, 0);
 *  char *str = lexer->get_string(lexer, startPos, numChars);
 *  lexer->free(lexer);
 */

// Include guards
#ifndef LEXER_H
#define LEXER_H

// Include libraries
#include <stdio.h>

/**
 *  @struct Lexer
 *  @brief  Reads characters from a string.
 */
typedef struct lexer {
    // Declare lexer fields
    const char *text;
    size_t      size;
    size_t      offset;
    size_t      line;
    size_t      column;

    // Declare lexer methods
    char    (*peek)       (const struct lexer *, long long);
    char    (*advance)    (struct lexer *);
    char   *(*get_string) (const struct lexer *, size_t, size_t);
    size_t  (*get_bound)  (const struct lexer *, long long);
    char    (*get_last)   (const struct lexer *);
    void    (*print)      (const struct lexer *);
    void    (*reset)      (struct lexer *);
    bool    (*ended)      (const struct lexer *);
    void    (*free)       (struct lexer *);
} Lexer;

// Declare functions
Lexer *lexer_init(FILE *);

// Include guard
#endif //LEXER_H