/**
 *  @file   file.c
 *  @brief  Converts files to strings and gets file information.
 *  @author Joel Nelems
 *  @date   2026-3-9
 */

// Include libraries
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "file.h"
#include "../../Error/error.h"
#define  PAGE_SIZE 0x1000

/**
 *  @fn    file_to_string
 *  @brief Converts a text file into a string and returns the string.
 *  @note  The string is dynamically allocated and must be freed by the caller.
 *  @note  This function will reset the file pointer to the beginning of the file.
 *
 *  @param  [in,out] file     The file to use. Must have read permissions.
 *  @param  [out]    numChars If set, stores the number of characters at the given address.
 *  @return The file text as a string, null-terminated.
 */
unsigned char *file_to_string(FILE *const file, size_t *const numChars) {
    // Check file is valid
    if (file == nullptr) {
        // Report error
        print_error(ERR_INVALID_FILE, true);
    }

    // Ensure file is reset
    rewind(file);

    // Create string
    size_t numPages = 1;
    unsigned char *text = malloc((PAGE_SIZE * numPages + 1) * sizeof(char));

    // Check malloc result
    if (text == nullptr) {
        // Report error
        print_error(ERR_MALLOC_FAIL, true);
    }

    // Fill string
    size_t charCount = 0;
    while (true) {
        // Check for overflow
        if (charCount >= PAGE_SIZE * numPages) {
            // Reallocate text
            numPages++;
            unsigned char *newText = realloc(text, PAGE_SIZE * numPages + 1);

            // Check realloc result
            if (newText == nullptr) {
                // Free text
                free(text);

                // Report error and exit
                print_error(ERR_MALLOC_FAIL, true);
            }

            // Update text pointer
            text = newText;
        }

        // Get character
        const int c = fgetc(file);

        // EOF check
        if (c == EOF) {
            // Exit loop
            break;
        }

        // Add character
        text[charCount++] = (unsigned char) c;
    }

    // Terminate
    text[charCount] = '\0';

    // Reset pointer
    rewind(file);

    // Check if char count is requested
    if (numChars != nullptr) {
        // Set character count
        *numChars = charCount;
    }

    // Return string
    return text;
}
