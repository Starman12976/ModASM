/**
 *  @file   instructionReader.h
 *  @brief  Reads through instructions.
 *  @author Joel Nelems
 *  @date   2026-3-9
 *
 *  @details
 *  TODO
 *
 *  @example
 *  TODO
 */

// Include guards
#ifndef INSTRUCTIONREADER_H
#define INSTRUCTIONREADER_H

// Include libraries
#include "../../Tokenization/TokenReader/tokenReader.h"
#include "../Instruction/instruction.h"

/**
 *  @struct InstructionReader
 *  @brief  Reads through a list of instructions.
 */
typedef struct instructionReader {
    // Declare instruction reader members
    const Instruction *const *instructions;
    size_t  numInstructions;
    size_t  offset;
    size_t  line;
    size_t  column;

    // Declare instruction reader methods
    Instruction  *(*peek)                (const struct instructionReader *, long long);
    Instruction  *(*advance)             (struct instructionReader *);
    Instruction **(*get_instruction_list)(const struct instructionReader *, size_t, size_t);
    size_t        (*get_bound)           (const struct instructionReader *, long long);
    Instruction  *(*get_last)            (const struct instructionReader *);
    void          (*print)               (const struct instructionReader *);
    void          (*reset)               (struct instructionReader *);
    bool          (*ended)               (const struct instructionReader *);
    void          (*free)                (struct instructionReader *);
} InstructionReader;

// Declare functions
InstructionReader *instruction_reader_init(TokenReader *);

// Include guard
#endif //INSTRUCTIONREADER_H