/**
 *  @file   tokenReader.h
 *  @brief  Stores and reads ModASM tokens.
 *  @author Joel Nelems
 *  @date   2025-10-29
 *
 *  @details
 *  The token reader structure reads through tokens using the advance() function. It automatically updates the
 *  position data (offset, line, column). It is recommended to allow the token reader to update its own values and not
 *  change them manually. Each token reader holds a pointer to its associated functions, allowing class-like methods.
 *  The token reader still has to be passed when calling them.
 *  The token reader has three primary functions:
 *  - peek(), which returns the token at the reader's current offset.
 *  - advance(), which returns the current token and increments the offset.
 *  - get_tokens(), which copies an array of tokens from the lexer's source tokens at a given position without updating
 *  the reader's position values. These methods are used for the instruction generation step of the assembler.
 *  Creating a token reader should be done through the token_reader_init() function. This function assigns a given token
 *  array to the reader and populates the rest of the values automatically, including the function pointers.
 *  To convert a lexer's text to a token array. use the get_tokens() function from the tokenizer.h script.
 *
 *  @example
 *  size_t tokenCount;
 *  Token **tokens = get_tokens(lexer, &tokenCount);
 *  TokenReader *tokenReader = token_reader_init(tokens, tokenCount);
 */

// Include guards
#ifndef TOKENREADER_H
#define TOKENREADER_H

// Include libraries
#include "../../Parsing/Lexer/lexer.h"
#include "../Token/token.h"

/**
 *  @struct TokenReader
 *  @brief  Reads from a list of tokens.
 */
typedef struct token_reader {
    // Declare token reader members
    const Token *const *tokens;
    size_t              numTokens;
    size_t              offset;
    size_t              line;
    size_t              column;

    // Declare token reader methods
    Token    *(*peek)           (const struct token_reader *, long long);
    Token    *(*advance)        (struct token_reader *);
    Token   **(*get_token_list) (const struct token_reader *, size_t, size_t);
    long long (*get_bound)      (const struct token_reader *, long long);
    Token    *(*get_last)       (const struct token_reader *);
    void      (*print)          (const struct token_reader *);
    void      (*reset)          (struct token_reader *);
    bool      (*ended)          (const struct token_reader *);
    void      (*free)           (struct token_reader *);
} TokenReader;

// Declare functions
TokenReader  *token_reader_init(Lexer *);

// Include guard
#endif //TOKENREADER_H