/**
 *  @file   error.h
 *  @brief  Keeps track of errors and prints error messages.
 *  @author Joel Nelems
 *  @date   2026-3-9
 *
 *  @details
 *  All errors have a code associated with them in the ErrorCode enum. To make error printing consistent and simple, the
 *  print_error() function only needs an error code and any associated data to output a message to stderr. If the error
 *  is set to fatal, the program will exit with the corresponding error code. Error codes that are not defined can be
 *  passed, but it will only output the error code without any further information.
 *
 *  @example
 *  if (!file) {
 *      print_error(ERR_CANNOT_OPEN, true, fileName);
 *  }
 */

// Include guards
#ifndef ERROR_H
#define ERROR_H

/**
 *  @enum  ErrorCode
 *  @brief Allows writing error codes as identifiers for readability.
 *
 *  @var ERR_WRONG_ARGC
 *  Returned when the program does not have the correct number of arguments.
 *
 *  @var ERR_CANNOT_OPEN
 *  Returned when a file that needs to be opened cannot be opened/found.
 *
 *  @var ERR_MALLOC_FAIL
 *  Returned whenever memory allocation fails.
 *
 *  @var ERR_INVALID_FILE
 *  Returned when a file passed to a function is NULL.
 *
 *  @var ERR_INVALID_LEXER
 *  Returned when a lexer was not initialized properly.
 *
 *  @var ERR_INVALID_TOKEN_READER
 *  Returned when a token reader was not initialized properly.
 *
 *  @var ERR_INVALID_INST_READER
 *  Returned when an instruction reader was not initialized properly.
 */
typedef enum error_code {
    // Input errors
    ERR_WRONG_ARGC = 1,
    ERR_CANNOT_OPEN,

    // Runtime errors
    ERR_MALLOC_FAIL,
    ERR_INVALID_FILE,
    ERR_INVALID_LEXER,
    ERR_INVALID_TOKEN_READER,
    ERR_INVALID_INST_READER,

    // Enum count
    ERR_COUNT
} ErrorCode; // Name enum

// Declare functions
void print_error(ErrorCode, bool, ...);

// Include guard
#endif //ERROR_H
