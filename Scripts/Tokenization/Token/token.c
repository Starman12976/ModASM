/**
 *  @file   token.h
 *  @brief  Creates the token structure.
 *  @author Joel Nelems
 *  @date   2025-11-03
 *
 *  @details
 *  The TokenType enum contains each possible keyword, literal, or symbol that will appear in a valid ModASM script. The
 *  get_tokens function uses a lexer to parse through the script and convert raw characters into tokens. Each token has
 *  an associated token type, a value based on the type, and some debug info like line and column.
 */

// Include libraries
#include <stdlib.h>
#include "token.h"
#include "../../Error/error.h"

// Declare functions
static void tok_free(Token *);

/**
 *  @fn    token_init
 *  @brief Creates a new token with the given field values.
 *  @note  The lexeme string should be dynamically allocated and freed using the token's free() method.
 *
 *  @param  [in] lexeme     The text of the token.
 *  @param  [in] type       The type of token.
 *  @param  [in] tokenIndex The token index in the script.
 *  @param  [in] value      The value of the token. Can be int, float, char, or str.
 *  @param  [in] line       The line this token appears on.
 *  @param  [in] column     The column this token begins on.
 *  @return The token created.
 */
Token *token_init(char *const lexeme, const TokenType type, const size_t tokenIndex, const TokValue value,
    const size_t line, const size_t column) {
    // Initialize token
    Token *const token = malloc(sizeof(Token));

    // Check malloc result
    if (token == nullptr) {
        // Report error
        print_error(ERR_MALLOC_FAIL, true);
    }

    // Set token fields
    token->lexeme     = lexeme;
    token->type       = type;
    token->tokenIndex = tokenIndex;
    token->value      = value;
    token->line       = line;
    token->column     = column;

    // Set token methods
    token->free = tok_free;

    // Return token
    return token;
}

/**
 *  @fn    tok_free
 *  @brief Frees the memory held by a token and its members.
 *
 *  @param  [in] token The token to free.
 *  @return Void.
 */
static void tok_free(Token *const token) {
    // Free token members
    free(token->lexeme);

    // Free token
    free(token);
}