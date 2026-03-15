/**
 *  @file   instruction.h
 *  @brief  Compiles tokens into instructions.
 *  @author Joel Nelems
 *  @date   2026-3-9
 *
 *  @details
 *  Each assembly instruction has an instruction type, like move, increment, or the system call. Each instruction can
 *  optionally have a destination that tells it what to act on, like a register or memory location. Instructions that
 *  have a destination can also have a source that is used to determine what value the instruction uses, which can be a
 *  register, memory, or literal. The Instruction type has pointers to the tokens that make up the instruction type,
 *  source, and destination that make up a whole instruction. If the instruction does not have a source or dest, the
 *  null pointer is used for those values.
 *
 *  @example
 *  Instruction *moveInst   = instruction_init(INST_MOVE, instTokens, numTokens, instIndex, instLine, instCol);
 */

// Include guards
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

// Include libraries
#include "../../Tokenization/Token/token.h"

/**
 *  @enum  InstructionType
 *  @brief Declares the types of instructions.
 */
typedef enum instruction_type {
    // Special
    INST_EOF = 0,
    INST_EMPTY,
    INST_ERROR,

    // Macros
    INST_REPLACE,

    // Declarations
    INST_DEFINE,
    INST_LET,

    // Movement
    INST_MOVE,

    // Calling
    INST_SYSCALL,
    INST_JUMP,
    INST_RETURN,

    // Count
    INST_COUNT
} InstructionType;

/**
 *  @struct Instruction
 *  @brief  Compiles tokens into an instruction that can be executed.
 */
typedef struct instruction {
    // Declare instruction members
    InstructionType type;
    Token         **tokens;
    size_t          numTokens;
    size_t          index;
    size_t          line;
    size_t          column;

    // Declare instruction methods
    void (*free)(struct instruction *);
} Instruction;

// Declare functions
Instruction *instruction_init(InstructionType, Token **, size_t, size_t, size_t, size_t);

// Include guard
#endif //INSTRUCTION_H
