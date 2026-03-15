/**
 *  @file   instruction.c
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
 */

// Include libraries
#include <stdlib.h>
#include "instruction.h"
#include "../../Error/error.h"

// Declare functions
static void inst_free(Instruction *);

/**
 *  @fn    instruction_init
 *  @brief Creates an instruction.
 *  @note  The structure is dynamically allocated and must be freed by the caller.
 *
 *  @param  [in] type      The instruction type.
 *  @param  [in] tokens    The tokens that make up the instruction.
 *  @param  [in] numTokens The number of tokens in the instruction.
 *  @param  [in] index     The index of the instruction.
 *  @param  [in] line      The row the instruction appears on.
 *  @param  [in] column    The column the instruction begins on.
 *  @return The created instruction.
 */
Instruction *instruction_init(const InstructionType type, Token **tokens, const size_t numTokens, const size_t index,
    const size_t line, const size_t column) {
    // Create new instruction
    Instruction *const instruction = malloc(sizeof(Instruction));

    // Check malloc result
    if (instruction == nullptr) {
        // Report error and exit
        print_error(ERR_MALLOC_FAIL, true);
    }

    // Set instruction members
    instruction->type      = type;
    instruction->tokens    = tokens;
    instruction->numTokens = numTokens;
    instruction->index     = index;
    instruction->line      = line;
    instruction->column    = column;

    // Set instruction methods
    instruction->free = inst_free;

    // Return instruction
    return instruction;
}

/**
 *  @fn    inst_free
 *  @brief Frees an instruction and its dynamically allocated members.
 *
 *  @param  [in] instruction The instruction to free.
 *  @return Void.
 */
static void inst_free(Instruction *instruction) {
    // Free instruction members
    free(instruction->tokens);

    // Free instruction
    free(instruction);
}