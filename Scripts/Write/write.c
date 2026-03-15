/**
 *  @file   write.c
 *  @brief  Writes values to a file.
 *  @author Joel Nelems
 *  @date   2025-10-08
 */

// Include libraries
#include <stdio.h>
#include "write.h"

/**
 *  @fn     write_byte
 *  @brief  Writes a byte to a binary file.
 *
 *  @param  [out] file The file to write to. Must be opened in binary write mode.
 *  @param  [in] value The byte to write.
 *  @return Void.
 */
void write_byte(FILE *file, const char value) {
    // Write byte
    fwrite(&value, 1, 1, file);
}

/**
 *  @fn     write_int
 *  @brief  Writes an integer of a given size to a file.
 *
 *  @param  [out] file The file to write to. Must be opened in binary write mode.
 *  @param  [in] value The value to write.
 *  @param  [in] size The number of bytes.
 *  @return Void.
 */
void write_int(FILE *file, const char value, const size_t size) {
    // Write int
    fwrite(&value, size, 1, file);
}