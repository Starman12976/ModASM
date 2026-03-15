/**
 *  @file   token.h
 *  @brief  Creates the token structure.
 *  @author Joel Nelems
 *  @date   2025-10-29
 *
 *  @details
 *  The TokenType enum contains each possible keyword, literal, or symbol that will appear in a valid ModASM script. The
 *  get_tokens function uses a lexer to parse through the script and convert raw characters into tokens. Each token has
 *  an associated token type, a value based on the type, and some debug info like line and column.
 *
 *  @example
 *  Token *token = token_init("12", TOK_INT_LIT, 20, TokValue { .int_value = 12 });
 */

// Include guards
#ifndef TOKEN_H
#define TOKEN_H

// Include libraries
#include "../../Parsing/Lexer/lexer.h"

/**
 *  @enum  TokenType
 *  @brief Allows writing token codes as identifiers for readability.
 */
typedef enum token_type {
    // Special
    TOK_EOF = 0,
    TOK_UNKNOWN,
    TOK_ERROR,

    // Instructions
    TOK_DEFINE,  // "define"
    TOK_LET,     // "let"
    TOK_MOVE,    // "move"
    TOK_TO,      // "to"
    TOK_SYSCALL, // "syscall"
    TOK_JUMP,    // "jump"
    TOK_RETURN,  // "return"
    TOK_REPLACE, // "replace"
    TOK_WITH,    // "with"
    TOK_NOTE,    // "note"

    // Identifiers and types
    TOK_IDENT,   // "x"
    TOK_SECTION, // "section"

    // Registers
    TOK_RAX, // "rax",
    TOK_RBX, // "rbx"
    TOK_RCX, // "rcx"
    TOK_RDX, // "rdx"
    TOK_RSI, // "rsi"
    TOK_RDI, // "rdi"

    // Delimiters
    TOK_EOI,         // '\n', ','
    TOK_BEGIN_SCOPE, // ':'
    TOK_END_SCOPE,   // '.'

    // Literals
    TOK_CHAR_LIT,   // "'a'"
    TOK_INT_LIT,    // "1", "-3"
    TOK_STRING_LIT, // ""abc""

    // Operators
    TOK_ASSIGN, // '='

    // Macros
    TOK_ARG_1,     // "sysArg1"
    TOK_ARG_2,     // "sysArg2"
    TOK_ARG_3,     // "sysArg3"
    TOK_ARG_4,     // "sysArg4"
    TOK_ARG_5,     // "sysArg5"
    TOK_ARG_6,     // "sysArg6"
    TOK_NEWLINE,   // "newline"
    TOK_STDOUT,    // "stdOut"
    TOK_SYS_EXIT,  // "sysExit"
    TOK_SYS_WRITE, // "sysWrite"

    // Count
    TOK_COUNT,
} TokenType;

/**
 *  @union  TokValue
 *  @brief  Declares the value of a token, which can be various types.
 */
typedef union tok_value {
    // Declare union members
    long long       int_value;
    double          float_value;
    unsigned char   char_value;
    unsigned char   symbol;
    const char     *string_value;
    const char     *identifier;
} TokValue;

/**
 *  @struct Token
 *  @brief  Stores token information for code generation.
 */
typedef struct token {
    // Declare token members
    TokenType   type;
    char       *lexeme;
    TokValue    value;
    size_t      line;
    size_t      column;
    size_t      tokenIndex;

    // Declare token methods
    void (*free)(struct token *);
} Token;

// Declare functions
Token *token_init(char *, TokenType, size_t, TokValue, size_t, size_t);

// Include guard
#endif //TOKEN_H
