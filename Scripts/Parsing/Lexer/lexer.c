/**
 *  @file   lexer.c
 *  @brief  Parses a ModASM file.
 *  @author Joel Nelems
 *  @date   2025-3-9
 */

// Include libraries
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "../../Error/error.h"
#include "../File/file.h"

// Declare functions
static char        peek       (const Lexer *, long long);
static char        advance    (Lexer *);
static char       *get_string (const Lexer *, size_t, size_t);
static size_t      get_bound  (const Lexer *, long long);
static char        get_last   (const Lexer *);
static void        reset      (Lexer *);
static void        print      (const Lexer *);
static bool        ended      (const Lexer *);
static void        lexer_free (Lexer *);

/**
 *  @fn    lexer_init
 *  @brief Creates a lexer structure with default values and methods.
 *  @note  The lexer is dynamically allocated and must be freed by the caller.
 *
 *  @param  [in, out] script The ModASM file.
 *  @return The lexer created.
 */
Lexer *lexer_init(FILE *script) {
    // Validate script
    if (script == nullptr) {
        // Report error
        print_error(ERR_INVALID_FILE, true);
    }

    // Initialize lexer
    Lexer *const self = malloc(sizeof(Lexer));

    // Check malloc result
    if (self == nullptr) {
        // Report error
        print_error(ERR_MALLOC_FAIL, true);
    }

    // Set lexer members
    self->text   = file_to_string(script, &(self->size));
    self->offset = 0;
    self->line   = 1;
    self->column = 1;

    // Set lexer methods
    self->peek       = peek;
    self->advance    = advance;
    self->get_string = get_string;
    self->get_bound  = get_bound;
    self->get_last   = get_last;
    self->print      = print;
    self->reset      = reset;
    self->ended      = ended;
    self->free       = lexer_free;

    // Return lexer
    return self;
}

/**
 *  @fn    peek
 *  @brief Gets the character the lexer is currently on, or a character relative to it.
 *
 *  @param  [in] self   The lexer to use.
 *  @param  [in] offset Gets the character relative to the lexer's current position.
 *  @return The character the lexer is on.
 */
static char peek(const Lexer *const self, const long long offset) {
    // Check for valid lexer
    if (self == nullptr) {
        // Print error
        print_error(ERR_INVALID_LEXER, true);
    }

    // Ensure offset is within bounds
    const size_t boundedOffset = self->get_bound(self, offset);

    // Return character
    return self->text[self->offset + boundedOffset];
}

/**
 *  @fn     advance
 *  @brief  Gets the character the lexer is currently on and increments its offset.
 *  @note   If incrementing the offset goes past the buffer size, it will return the last character and not increment.
 *
 *  @param  [in,out] self The lexer to use.
 *  @return The character the lexer is on.
 */
static char advance(Lexer *const self) {
    // Check for valid lexer
    if (self == nullptr) {
        // Print error
        print_error(ERR_INVALID_LEXER, true);
    }

    // Check bounds
    if (self->ended(self)) {
        // Return last character
        return self->get_last(self);
    }

    // Get current character
    const char currChar = self->text[self->offset++];

    // Update lexer position
    switch (currChar) {
        // Newline
        case '\n': {
            // Update position
            self->line++;
            self->column = 1;

            // Exit switch
            break;
        }

        // Return
        case '\r': {
            // Update position
            self->column = 1;

            // Exit switch
            break;
        }

        // Default
        default: {
            // Update position
            self->column++;

            // Exit switch
            break;
        }
    }

    // Return character
    return currChar;
}

/**
 *  @fn    get_string
 *  @brief Gets a string from the lexer at a given position and with a provided length.
 *  @note  The string is dynamically allocated and must be freed by the caller.
 *
 *  @param  [in] self     The lexer to use.
 *  @param  [in] start    The offset to begin from.
 *  @param  [in] numChars The number of characters to read.
 *  @return The string found.
 */
static char *get_string(const Lexer *const self, const size_t start, size_t numChars) {
    // Check for valid lexer
    if (self == nullptr) {
        // Print error
        print_error(ERR_INVALID_LEXER, true);
    }

    // Ensure start is within bounds
    if (start >= self->size) {
        // Return empty string
        return strdup("");
    }

    // Ensure size is within bounds
    if (start + numChars >= self->size) {
        // Read until EOF
        numChars = self->size - start;
    }

    // Create string
    char *const lexeme = malloc(sizeof(char) * (numChars + 1));

    // Check malloc result
    if (lexeme == nullptr) {
        // Report error
        print_error(ERR_MALLOC_FAIL, true);
    }

    // Fill string
    memcpy(lexeme, self->text + start, numChars);

    // Terminate
    lexeme[numChars] = '\0';

    // Return string
    return lexeme;
}

/**
 *  @fn    get_bound
 *  @brief Takes an offset and returns the lower/upper bounds if it goes out of the range of the lexer's text.
 *
 *  @param  [in] self   The lexer to use.
 *  @param  [in] offset The offset relative to the current position.
 *  @return The valid index.
 */
static size_t get_bound(const Lexer *const self, const long long offset) {
    // Check upper bound
    if (self->offset + offset >= self->size) {
        // Return last character
        return self->size - self->offset;
    }

    // Check lower bound
    if (self->offset + offset < 0) {
        // Return first character
        return -self->offset;
    }

    // Return valid offset
    return offset;
}

/**
 *  @fn    get_last
 *  @brief Gets the last character in the lexer's text.
 *
 *  @param  [in] self The lexer to use.
 *  @return The last character.
 */
char get_last(const Lexer *const self) {
    // Return last character
    return self->text[self->size - 1];
}

/**
 *  @fn     ended
 *  @brief  Returns true if the lexer has reached the end of its text.
 *
 *  @param  [in] self The lexer to use.
 *  @return True if the lexer has reached the end, otherwise false.
 */
static bool ended(const Lexer *const self) {
    // Return true at end
    if (self->offset >= self->size) {
        // Return true
        return true;
    }

    // Return false
    return false;
}

/**
 *  @fn    reset
 *  @brief Resets the offset and position data of the lexer.
 *
 *  @param  [in] self The lexer to use.
 *  @return Void.
 */
static void reset(Lexer *const self) {
    // Check for valid self
    if (self == nullptr) {
        // Print error
        print_error(ERR_INVALID_LEXER, true);
    }

    // Reset values
    self->offset   = 0;
    self->line     = 1;
    self->column   = 1;
}

/**
 *  @fn    print
 *  @brief Prints out the characters stored in the lexer's text.
 *
 *  @param  [in] self The lexer to use.
 *  @return Void.
 */
static void print(const Lexer *const self) {
    // Check for valid lexer
    if (self == nullptr) {
        // Print error
        print_error(ERR_INVALID_LEXER, true);
    }

    // Loop through characters
    for (size_t charIndex = 0; charIndex < self->size; charIndex++) {
        // Output character
        printf("%c", self->text[charIndex]);
    }

    // Output newline
    printf("\n");
}

/**
 *  @fn     lexer_free
 *  @brief  Deallocates the memory for the provided lexer and all its members.
 *  @note   The lexer must have been initialized with a dynamically allocated string.
 *
 *  @param  [out] self The lexer to use.
 *  @return Void.
 */
static void lexer_free(Lexer *const self) {
    // Free members
    free((void *) self->text);

    // Free self
    free(self);
}