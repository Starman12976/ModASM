/**
 *  @file   tokenReader.h
 *  @brief  Stores and reads ModASM tokens.
 *  @author Joel Nelems
 *  @date   2026-3-9
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
 */

// Include libraries
#include <stdlib.h>
#include "tokenReader.h"
#include "../Token/token.h"
#include "../Tokenizer/tokenizer.h"
#include "../../Error/error.h"

// Declare functions
static const Token  *peek              (const TokenReader *, long long);
static const Token  *advance           (TokenReader *);
static const Token **get_token_list    (const TokenReader *, size_t, size_t);
static long long     get_bound         (const TokenReader *, long long);
const Token         *get_last          (const TokenReader *);
static void          reset             (TokenReader *);
static void          print             (const TokenReader *);
static bool          ended             (const TokenReader *);
static void          free_token_reader (TokenReader *);

/**
 *  @var   TokenTypeStrs
 *  @brief Allows writing token types as strings for printing.
 */
const char *const TokenTypeStrs[TOK_COUNT] = {
    // Special
    [TOK_EOF]     = "EOF",
    [TOK_UNKNOWN] = "UNKNOWN",
    [TOK_ERROR]   = "ERROR",

    // Instructions
    [TOK_DEFINE]  = "DEFINE",
    [TOK_LET]     = "LET",
    [TOK_MOVE]    = "MOVE",
    [TOK_TO]      = "TO",
    [TOK_SYSCALL] = "SYSCALL",
    [TOK_JUMP]    = "JUMP",
    [TOK_RETURN]  = "RETURN",
    [TOK_REPLACE] = "REPLACE",
    [TOK_WITH]    = "WITH",
    [TOK_NOTE]    = "NOTE",

    // Identifiers and types
    [TOK_IDENT]   = "IDENT",
    [TOK_SECTION] = "SECTION",

    // Registers
    [TOK_RAX] = "RAX",
    [TOK_RBX] = "RBX",
    [TOK_RCX] = "RCX",
    [TOK_RDX] = "RDX",
    [TOK_RSI] = "RSI",
    [TOK_RDI] = "RDI",

    // Delimiters
    [TOK_EOI]         = "EOI",
    [TOK_BEGIN_SCOPE] = "BEGIN SCOPE",
    [TOK_END_SCOPE]   = "END SCOPE",

    // Literals
    [TOK_CHAR_LIT]   = "CHAR LIT",
    [TOK_INT_LIT]    = "INT LIT",
    [TOK_STRING_LIT] = "STRING LIT",

    // Operators
    [TOK_ASSIGN] = "ASSIGN",

    // Macros
    [TOK_ARG_1]     = "ARG 1",
    [TOK_ARG_2]     = "ARG 2",
    [TOK_ARG_3]     = "ARG 3",
    [TOK_ARG_4]     = "ARG 4",
    [TOK_ARG_5]     = "ARG 5",
    [TOK_ARG_6]     = "ARG 6",
    [TOK_NEWLINE]   = "NEWLINE",
    [TOK_STDOUT]    = "STDOUT",
    [TOK_SYS_WRITE] = "SYS WRITE",
    [TOK_SYS_EXIT]  = "SYS EXIT",
};

/**
 *  @fn     token_reader_init
 *  @brief  Creates a token reader using a lexer.
 *
 *  @param  [in, out] lexer The lexer to use.
 *  @return The token reader created.
 */
TokenReader *token_reader_init(Lexer *lexer) {
    // Verify lexer
    if (lexer == nullptr) {
        // Report error and exit
        print_error(ERR_INVALID_LEXER, true);
    }

    // Create new token reader
    TokenReader *const self = malloc(sizeof(TokenReader));

    // Check malloc result
    if (self == nullptr) {
        // Report error
        print_error(ERR_MALLOC_FAIL, true);
    }

    // Define token reader members
    self->tokens = get_tokens(lexer, &(self->numTokens));
    self->offset = 0;
    self->line   = self->tokens[0]->line;
    self->column = self->tokens[0]->column;

    // Set token reader methods
    self->peek           = peek;
    self->advance        = advance;
    self->get_token_list = get_token_list;
    self->get_bound      = get_bound;
    self->get_last       = get_last;
    self->print          = print;
    self->reset          = reset;
    self->ended          = ended;
    self->free           = free_token_reader;

    // Return token reader
    return self;
}

/**
 *  @fn     peek
 *  @brief  Gets a token relative to the token reader's current offset.
 *
 *  @param  [in] self   The token reader to use.
 *  @param  [in] offset The offset from the token reader's current position.
 *  @return The token found.
 */
static const Token *peek(const TokenReader *const self, const long long offset) {
    // Check for valid token reader
    if (self == nullptr) {
        // Print error and exit
        print_error(ERR_INVALID_TOKEN_READER, true);
    }

    // Ensure offset is within bounds
    const size_t boundedOffset = self->get_bound(self, offset);

    // Return the token at the given offset
    return self->tokens[self->offset + boundedOffset];
}

/**
 *  @fn     advance
 *  @brief  Gets the token that the token reader is currently on and increments its offset.
 *
 *  @param  [in, out] self The token reader to use.
 *  @return The current token.
 */
static const Token *advance(TokenReader *const self) {
    // Check for valid token reader
    if (self == nullptr) {
        // Print error and exit
        print_error(ERR_INVALID_TOKEN_READER, true);
    }

    // Check bounds
    if (self->ended(self)) {
        return self->get_last(self);
    }

    // Get current token and increment offset
    const Token *const currToken = self->tokens[self->offset++];

    // Update reader position
    self->line   = currToken->line;
    self->column = currToken->column;

    // Return token
    return currToken;
}

/**
 *  @fn     get_tokens
 *  @brief  Gets a set of tokens starting at an offset.
 *  @note   The array of tokens is dynamically allocated and must be freed by the caller.
 *
 *  @param  [in] self      The token reader to use.
 *  @param  [in] start     The offset to the first token from the start of the token list.
 *  @param  [in] numTokens The number of tokens to read.
 *  @return The array of tokens created.
 */
static const Token **get_token_list(const TokenReader *const self, const size_t start, size_t numTokens) {
    // Check for valid token reader
    if (self == nullptr) {
        // Print error and exit
        print_error(ERR_INVALID_TOKEN_READER, true);
    }

    // Check bounds
    if (start >= self->numTokens) {
        // Return empty token list
        Token **const tokens = malloc(0);
        return tokens;
    }

    // Check token count
    if (start + numTokens >= self->numTokens) {
        // Clamp token count
        numTokens = self->numTokens - start;
    }

    // Initialize token list
    const Token **const tokens = malloc(sizeof(Token *) * numTokens);

    // Loop through indices
    for (size_t tokenIndex = 0; tokenIndex < numTokens; tokenIndex++) {
        // Add token to list
        tokens[tokenIndex] = self->tokens[start + tokenIndex];
    }

    // Return token list
    return tokens;
}

/**
 *  @fn    get_bound
 *  @brief Takes an offset and returns the lower/upper bounds if it goes out of the range of the token reader's tokens.
 *
 *  @param  [in] self   The token reader to use.
 *  @param  [in] offset The offset relative to the current position.
 *  @return The valid index.
 */
static long long get_bound(const TokenReader *const self, const long long offset) {
    // Check for valid token reader
    if (self == nullptr) {
        // Print error and exit
        print_error(ERR_INVALID_TOKEN_READER, true);
    }

    // Check upper bound
    if (self->offset + offset >= self->numTokens) {
        // Return last token index
        return (long long) self->numTokens - (long long) self->offset - 1;
    }

    // Check lower bound
    if (self->offset + offset < 0) {
        // Return first token index
        return -(long long)self->offset;
    }

    // Return valid offset
    return offset;
}

/**
 *  @fn    get_last
 *  @brief Gets the last token in the token reader's token list.
 *
 *  @param  [in] self The token reader to use.
 *  @return The last token.
 */
const Token *get_last(const TokenReader *const self) {
    // Check for valid token reader
    if (self == nullptr) {
        // Print error and exit
        print_error(ERR_INVALID_TOKEN_READER, true);
    }

    // Return last character
    return self->tokens[self->numTokens - 1];
}

/**
 *  @fn     ended
 *  @brief  Returns true if the token reader has reached the end of its token list.
 *
 *  @param  [in] self The token reader to use.
 *  @return True if the reader has reached the end, otherwise false.
 */
static bool ended(const TokenReader *const self) {
    // Check for valid token reader
    if (self == nullptr) {
        // Print error and exit
        print_error(ERR_INVALID_TOKEN_READER, true);
    }

    // Return true at end
    if (self->offset >= self->numTokens - 1) {
        // Return true
        return true;
    }

    // Return false
    return false;
}

/**
 *  @fn    reset
 *  @brief Resets the offset and position data of the token reader.
 *
 *  @param  [in] self The token reader to use.
 *  @return Void.
 */
static void reset(TokenReader *const self) {
    // Check for valid token reader
    if (self == nullptr) {
        // Print error
        print_error(ERR_INVALID_TOKEN_READER, true);
    }

    // Reset values
    self->offset = 0;
    self->line   = self->tokens[0]->line;
    self->column = self->tokens[0]->column;
}

/**
 *  @fn     print
 *  @brief  Prints the tokens in a token reader.
 *
 *  @param  [in] self The token reader to use.
 *  @return Void.
 */
static void print(const TokenReader *const self) {
    // Check for valid token reader
    if (self == nullptr) {
        // Print error
        print_error(ERR_INVALID_TOKEN_READER, true);
    }

    // Loop through tokens
    for (const Token *const *tokenP = self->tokens; (*tokenP)->type != TOK_EOF; tokenP++) {
        // Get token as string
        const char *tokStr = TokenTypeStrs[(*tokenP)->type] != nullptr ? TokenTypeStrs[(*tokenP)->type] : "Undefined";

        // Print token info
        printf("Index: %-3llu | Type: %-14s | Lexeme: %s\n",
            (*tokenP)->tokenIndex, tokStr, (*tokenP)->lexeme[0] == '\n' ? "newline" : (*tokenP)->lexeme);
    }
}

/**
 *  @fn     free_token_reader
 *  @brief  Frees a token reader and the tokens contained within it.
 *
 *  @param  [in] tokenReader The token reader to free.
 *  @return Void.
 */
static void free_token_reader(TokenReader *tokenReader) {
    // Loop through tokens
    for (size_t tokenIndex = 0; tokenIndex < tokenReader->numTokens; tokenIndex++) {
        // Free token
        tokenReader->tokens[tokenIndex]->free(tokenReader->tokens[tokenIndex]);
    }

    // Free token reader
    free(tokenReader);
}