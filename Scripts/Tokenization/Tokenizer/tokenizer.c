/**
 *  @file   tokenizer.c
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
 */

// Include libraries
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "../Token/token.h"
#include "../../Error/error.h"
#include "../../Parsing/Lexer/lexer.h"
#define  CHUNK_SIZE 0x100

// Declare functions
static Token *get_token(Lexer *lexer, size_t tokenIndex);

/**
 *  @var   keywords
 *  @brief Holds the matching keyword string for each keyword token type.
 */
static const char *const keywords[TOK_COUNT] = {
    // Instructions
    [TOK_DEFINE]    = "define",
    [TOK_LET]       = "let",
    [TOK_MOVE]      = "move",
    [TOK_TO]        = "to",
    [TOK_SYSCALL]   = "syscall",
    [TOK_JUMP]      = "jump",
    [TOK_RETURN]    = "return",
    [TOK_REPLACE]   = "replace",
    [TOK_WITH]      = "with",
    [TOK_NOTE]      = "note",

    // Types
    [TOK_SECTION] = "section",

    // Registers
    [TOK_RAX] = "rax",
    [TOK_RBX] = "rbx",
    [TOK_RCX] = "rcx",
    [TOK_RDX] = "rdx",
    [TOK_RSI] = "rsi",
    [TOK_RDI] = "rdi",

    // Macros
    [TOK_ARG_1]     = "sysarg1",
    [TOK_ARG_2]     = "sysarg2",
    [TOK_ARG_3]     = "sysarg3",
    [TOK_ARG_4]     = "sysarg4",
    [TOK_ARG_5]     = "sysarg5",
    [TOK_ARG_6]     = "sysarg6",
    [TOK_NEWLINE]   = "newline",
    [TOK_STDOUT]    = "stdout",
    [TOK_SYS_EXIT]  = "sysexit",
    [TOK_SYS_WRITE] = "syswrite",
};

/**
 *  @fn    is_ident_char
 *  @brief Determines if a character is a valid identifier character.
 *
 *  @param  [in] character The character to use.
 *  @param  [in] is_first Doesn't allow digits if this is true.
 *  @return True if the character is valid, otherwise false.
 */
static bool is_ident_char(const unsigned char character, const bool is_first) {
    // All alphanumeric characters and underscore are allowed, no digits on first character
    if ((isalnum(character) || character == '_') && !(is_first && isdigit(character))) {
        // Return valid
        return true;
    }

    // Return invalid
    return false;
}

/**
 *  @fn     get_key
 *  @brief  Given a string, returns the corresponding token type.
 *
 *  @param  [in] string The string to use.
 *  @return The corresponding TokenType code.
 */
static TokenType get_key(const char *const string) {
    // Loop through defined keywords
    for (size_t keyIndex = 0; keyIndex < TOK_COUNT; keyIndex++) {
        // Skip NULL
        if (keywords[keyIndex] == nullptr) {
            // Continue
            continue;
        }

        // Compare provided string against keyword string
        if (strcmp(string, keywords[keyIndex]) == 0) {
            // Return matching type
            return keyIndex;
        }
    }

    // No type found, return identifier
    return TOK_IDENT;
}

/**
 *  @fn parse_ident
 *  @brief Reads an identifier using a lexer and returns the corresponding token.
 *
 *  @param  [in,out] lexer The lexer to use.
 *  @param  [in] tokenIndex Index of the current token.
 *  @return The token generated.
 */
static Token *parse_ident(Lexer *const lexer, const size_t tokenIndex) {
    // Save lexeme info
    const size_t line   = lexer->line;
    const size_t column = lexer->column;
    const size_t start  = lexer->offset;
    size_t tokenSize    = 0;

    // Read identifier characters
    while (is_ident_char(lexer->peek(lexer, 0), tokenSize == 0 ? true : false)) {
        // Increment size
        tokenSize++;

        // Advance lexer
        lexer->advance(lexer);
    }

    // Get lexeme
    char *const lexeme = lexer->get_string(lexer, start, tokenSize);

    // Initialize token
    const TokValue tokValue  = { .identifier = lexeme };
    Token *const token = token_init(lexeme, get_key(lexeme), tokenIndex, tokValue, line, column);

    // Return token
    return token;
}

/**
 *  @fn     parse_number
 *  @brief  Reads a number using a lexer and returns the corresponding token.
 *
 *  @param  [in,out] lexer The lexer to use.
 *  @param  [in] tokenIndex The index of the current token.
 *  @return The token generated.
 */
static Token *parse_number(Lexer *const lexer, const size_t tokenIndex) {
    // Save lexeme info
    const size_t line   = lexer->line;
    const size_t column = lexer->column;
    const size_t start  = lexer->offset;
    size_t tokenSize    = 0;

    // Read identifier characters
    while (isdigit(lexer->peek(lexer, 0))) {
        // Increment size
        tokenSize++;

        // Advance
        lexer->advance(lexer);
    }

    // Get lexeme
    char *const lexeme = lexer->get_string(lexer, start, tokenSize);

    // Initialize token
    const TokValue tokValue     = { .int_value = strtol(lexeme, nullptr, 10) };
    Token *const token    = token_init(lexeme, TOK_INT_LIT, tokenIndex, tokValue, line, column);

    // Return token
    return token;
}

/**
 *  @fn    parse_char
 *  @brief Reads a character using a lexer and returns the corresponding token.
 *
 *  @param  [in,out] lexer The lexer to use.
 *  @param  [in] tokenIndex Index of the current token.
 *  @return The token generated.
 */
static Token *parse_char(Lexer *const lexer, const size_t tokenIndex) {
    // Get lexeme info
    const size_t line   = lexer->line;
    const size_t column = lexer->column;
    const size_t start  = lexer->offset;
    size_t numChars     = 0;

    // Loop until end of character
    size_t numQuote = 0;
    while (numQuote != 2) {
        // Increment character count and advance
        numChars++;
        const char c = lexer->advance(lexer);

        // Check for quote
        if (c == '\'') {
            // Increment quote count
            numQuote++;
        }
    }

    // Get lexeme
    char *const lexeme = lexer->get_string(lexer, start, numChars);

    // Initialize token
    const TokValue tokValue  = { .char_value = lexeme[1] };
    Token *const token = token_init(lexeme, TOK_CHAR_LIT, tokenIndex, tokValue, line, column);

    // Return token
    return token;
}

/**
 *  @fn    parse_string
 *  @brief Reads a string using a lexer and returns the corresponding token.
 *
 *  @param  [in,out] lexer The lexer to use.
 *  @param  [in] tokenIndex Index of the current token.
 *  @return The token generated.
 */
static Token *parse_string(Lexer *const lexer, const size_t tokenIndex) {
    // Save lexeme info
    const size_t line   = lexer->line;
    const size_t column = lexer->column;
    const size_t start  = lexer->offset;
    size_t stringLength = 0;

    // Calculate string size
    size_t numQuote = 0;
    while (numQuote != 2) {
        // Increment and advance
        stringLength++;
        const char c = lexer->advance(lexer);

        // Look for quote
        if (c == '\"') {
            // Increment quote count
            numQuote++;
        }
    }

    // Get lexeme
    char *const lexeme = lexer->get_string(lexer, start, stringLength);

    // Initialize token
    const TokValue tokValue  = { .string_value = lexeme };
    Token *const token = token_init(lexeme, TOK_STRING_LIT, tokenIndex, tokValue, line, column);

    // Return token
    return token;
}

/**
 *  @fn    parse_symbol
 *  @brief Reads a symbol using a lexer and returns the corresponding token.
 *
 *  @param  [in,out] lexer The lexer to use.
 *  @param  [in] tokenIndex Index of the current token.
 *  @return The token generated.
 */
static Token *parse_symbol(Lexer *const lexer, const size_t tokenIndex) {
    // Get lexeme info
    const size_t line     = lexer->line;
    const size_t column   = lexer->column;
    const size_t offset   = lexer->offset;
    const size_t numChars = 1;

    // Get type
    TokenType type;
    switch (lexer->peek(lexer, 0)) {
        // Assignment
        case '=': {
            // Set type
            type = TOK_ASSIGN;

            // Exit switch
            break;
        }

        // Begin scope
        case ':': {
            // Set type
            type = TOK_BEGIN_SCOPE;

            // Exit switch
            break;
        }

        // End of scope
        case '.': {
            // Set type
            type = TOK_END_SCOPE;

            // Exit switch
            break;
        }

        // End of instruction
        case '\n':
        case  ',': {
            // Set type
            type = TOK_EOI;

            // Exit switch
            break;
        }

        // Default
        default: {
            // Set type
            type = TOK_UNKNOWN;

            // Exit switch
            break;
        }
    }

    // Get lexeme
    char *const lexeme = lexer->get_string(lexer, offset, numChars);

    // Advance
    lexer->advance(lexer);

    // Initialize token
    const TokValue tokValue  = { .symbol = lexeme[0] };
    Token *const token = token_init(lexeme, type, tokenIndex, tokValue, line, column);

    // Return token
    return token;
}

/**
 *  @brief Gets the next token in a ModASM script using a lexer.
 *
 *  @param  [in,out] lexer The lexer to use.
 *  @param  [in] tokenIndex The index of the current token.
 *  @return The generated token.
 */
static Token *get_token(Lexer *const lexer, const size_t tokenIndex) {

    // Skip space
    while (isspace(lexer->peek(lexer, 0)) && lexer->peek(lexer, 0) != '\n') {
        // Skip space
        lexer->advance(lexer);
    }

    // EOF check
    if (lexer->ended(lexer)) {
        // Generate EOF token
        const TokValue tokValue = { .int_value = -1 };
        const char    *lexeme = strdup("");
        Token *token = token_init(lexeme, TOK_EOF, tokenIndex, tokValue, lexer->line, lexer->column);

        // Return EOF token
        return token;
    }

    // Get current character
    const char currChar = lexer->peek(lexer, 0);

    // Parse identifiers
    if (is_ident_char(currChar, true)) {
        // Parse identifier
        return parse_ident(lexer, tokenIndex);
    }

    // Parse numbers
    if (isdigit(currChar)) {
        // Parse number
        return parse_number(lexer, tokenIndex);
    }

    // Parse characters
    if (currChar == '\'') {
        // Parse character
        return parse_char(lexer, tokenIndex);
    }

    // Parse strings
    if (currChar == '\"') {
        // Parse string
        return parse_string(lexer, tokenIndex);
    }

    // Return symbol
    return parse_symbol(lexer, tokenIndex);
}

Token *const *get_tokens(Lexer *const lexer, size_t *const numTokens) {
    // Initialize token list
    size_t  numChunks = 1;
    Token **tokens    = malloc(sizeof(Token *) * CHUNK_SIZE);

    // Check malloc result
    if (tokens == nullptr) {
        // Report error
        print_error(ERR_MALLOC_FAIL, true);
    }

    // Reset lexer
    lexer->reset(lexer);

    // Loop through tokens
    size_t tokenCount = 0;
    while (true) {
        // Check array size
        if (tokenCount >= CHUNK_SIZE * numChunks) {
            // Reallocate token array
            numChunks++;
            Token **newTokens = realloc(tokens, sizeof(Token *) * CHUNK_SIZE * numChunks);

            // Check realloc result
            if (newTokens == nullptr) {
                // Free old array
                free(tokens);

                // Report error and exit
                print_error(ERR_MALLOC_FAIL, true);
            }

            // Update tokens pointer
            tokens = newTokens;
        }

        // Get token
        Token *token = get_token(lexer, tokenCount);

        // Add to list
        tokens[tokenCount++] = token;

        // Check for EOF
        if (token->type == TOK_EOF) {
            // Exit loop
            break;
        }

        // Check for note
        if (token->type == TOK_NOTE) {
            // Skip until newline or EOF
            while (lexer->peek(lexer, 0) != '\n' && !lexer->ended(lexer)) {
                // Advance lexer
                lexer->advance(lexer);
            }
        }
    }

    // Check if token count is requested
    if (numTokens != nullptr) {
        // Store token count
        *numTokens = tokenCount;
    }

    // Return token list
    return tokens;
}