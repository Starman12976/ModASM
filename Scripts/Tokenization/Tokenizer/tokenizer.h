/**
 *  @file   tokenizer.h
 *  @brief  Converts a string into an array of tokens using a lexer.
 *  @author Joel Nelems
 *  @date   2025-10-29
 *
 *  @details
 *  The get_tokens() function uses an initialized lexer to read through a ModASM script and convert it into an array of
 *  tokens. There are three types of tokens: Keywords/identifiers, literals, and symbols. A keyword or identifier is
 *  defined as any string of alphanumeric characters (but doesn't start with a digit). Literals include integer/float
 *  values and character/string literals, denoted with the '' and "" symbols, respectively. Symbols are any set of
 *  characters that are not alphanumeric and not whitespace. The tokenizer uses these rules to split up the text of a
 *  lexer into tokens.
 *
 *  @example
 *  size_t  tokenCount;
 *  Lexer  *lexer  = lexer_init(fp);
 *  Token **tokens = get_tokens(lexer, &tokenCount);
 */

// Include guards
#ifndef TOKENIZER_H
#define TOKENIZER_H

// Include libraries
#include "../Token/token.h"

// Declare functions
Token *const *get_tokens(Lexer *, size_t *);

// Include guard
#endif //TOKENIZER_H