/**
 *  @file   error.c
 *  @brief  Keeps track of errors and prints error messages.
 *  @author Joel Nelems
 *  @date   2026-3-9
 */

// Include libraries
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

/**
 *  @var   errors
 *  @brief Stores the message of each error code.
 *
 *  @details
 *  This table is used by indexing with an error code to get the associated error message. print_error uses the
 *  associated element's message for printing.
 */
static const char *const error_messages[ERR_COUNT] = {
    // Input errors
    [ERR_WRONG_ARGC]  = "Invalid argument count\nUsage: user:~$ ModASM file.asm\n",
    [ERR_CANNOT_OPEN] = "Unable to open file: %s\n",

    // Runtime errors
    [ERR_MALLOC_FAIL]          = "Memory allocation failed\n",
    [ERR_INVALID_FILE]         = "Invalid file pointer\n",
    [ERR_INVALID_LEXER]        = "Invalid lexer\n",
    [ERR_INVALID_TOKEN_READER] = "Invalid token reader\n",
    [ERR_INVALID_INST_READER]  = "Invalid instruction reader\n"
};

/**
 *  @fn     print_error
 *  @brief  Prints an error using a given code and format arguments. Exits if the error is fatal.
 *
 *  @param  [in] code The error code to use.
 *  @param  [in] isFatal Exits the program if set to true.
 *  @param  [in] ... Format arguments to provide extra information about an error.
 *  @return Void.
 */
void print_error(const ErrorCode code, const bool isFatal, ...) {
    // Check for undefined code
    if (code >= ERR_COUNT) {
        // Report unknown error
        fprintf(stderr, "Error %d: Unknown error.\n", code);

        // Check for fatal error
        if (isFatal) {
            // Exit
            exit(code);
        }

        // Return
        return;
    }

    // Get error
    const char *const error_message = error_messages[code];

    // Print error code
    fprintf(stderr, "Error %d: ", code);

    // Get format arguments
    va_list args;
    va_start(args, isFatal);

    // Print message
    vfprintf(stderr, error_message, args);

    // End arg list
    va_end(args);

    // Exit if error is fatal
    if (isFatal) {
        // Exit with error code
        exit(code);
    }
}