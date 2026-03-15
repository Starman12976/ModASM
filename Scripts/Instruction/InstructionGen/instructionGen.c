/**
 *  @file   instructionGen.c
 *  @brief  Uses a Token Reader to generate an instruction list.
 *  @author Joel Nelems
 *  @date   2026-3-9
 *
 *  @details
 *  TODO
 */

// Include libraries
#include <stdlib.h>
#include "instructionGen.h"
#include "../../Error/error.h"

// Define macros
#define CHUNK_SIZE 0x40

/**
 *  @fn    get_inst
 *  @brief Gets the next instruction from a token reader.
 *
 *  @param  [in, out] reader The token reader to use.
 *  @param  [in]      instIndex    The index of the current instruction.
 *  @return The created instruction.
 */
static Instruction *get_inst(TokenReader *reader, const size_t instIndex) {
    // EOF check
    if (reader->ended(reader)) {
        // Create EOF instruction
        Instruction *inst = instruction_init(INST_EOF, nullptr, 0, instIndex, reader->get_last(reader)->line,
                                                reader->get_last(reader)->column);

        // Return instruction
        return inst;
    }

    // Get current token
    const Token    *token = reader->peek(reader, 0);
    InstructionType instType;

    // Match token type
    switch (token->type) {
        // EOI token / note instruction (empty)
        case TOK_EOI:
        case TOK_NOTE: {
            // Parse eoi/note
            instType = INST_EMPTY;

            // Exit switch
            break;
        }

        // Replace instruction
        case TOK_REPLACE: {
            // Parse replace
            instType = INST_REPLACE;

            // Exit switch
            break;
        }

        // Define instructions
        case TOK_DEFINE: {
            // Parse define
            instType = INST_DEFINE;

            // Exit switch
            break;
        }

        // Let instructions
        case TOK_LET: {
            // Parse define
            instType = INST_LET;

            // Exit switch
            break;
        }

        // Move instruction
        case TOK_MOVE: {
            // Parse move
            instType = INST_MOVE;

            // Exit switch
            break;
        }

        // Syscall instruction
        case TOK_SYSCALL: {
            // Parse syscall
            instType = INST_SYSCALL;

            // Exit switch
            break;
        }

        // Return instruction
        case TOK_RETURN: {
            // Parse return
            instType = INST_RETURN;

            // Exit switch
            break;
        }

        // Jump instruction
        case TOK_JUMP: {
            // Parse jump
            instType = INST_JUMP;

            // Exit switch
            break;
        }

        // Unknown instruction
        default: {
            // Create error token
            instType = INST_ERROR;

            // Exit switch
            break;
        }
    }

    // Save instruction info
    size_t       numInstructions  = 0;
    const size_t startIndex       = reader->offset;
    const size_t instLine         = reader->line;
    const size_t instColumn       = reader->column;

    // Read until EOI/EOF
    do {
        // Advance reader
        reader->advance(reader);

        // Increment token count
        numInstructions++;
    } while (reader->peek(reader, -1)->type != TOK_EOI && !reader->ended(reader));

    // Get token list
    Token **tokenList = reader->get_token_list(reader, startIndex, numInstructions + 1);

    // Create instruction
    Instruction *inst = instruction_init(instType, tokenList, numInstructions, instIndex, instLine, instColumn);

    // Return instruction
    return inst;
}

/**
 *  @fn    get_instructions
 *  @brief TODO
 *
 *  @param  [in, out] reader          TODO
 *  @param  [out]     numInstructions TODO
 *  @return TODO
 */
Instruction **get_instructions(TokenReader *reader, size_t *numInstructions) {
    // Initialize instruction list
    size_t        numChunks    = 1;
    Instruction **instructions = malloc(sizeof(Instruction *) * CHUNK_SIZE);

    // Check malloc result
    if (instructions == nullptr) {
        // Report error and exit
        print_error(ERR_MALLOC_FAIL, true);
    }

    // Reset token reader
    reader->reset(reader);

    // Loop through instructions
    size_t instCount = 0;
    while (true) {
        // Check array size
        if (instCount >= CHUNK_SIZE * numChunks) {
            // Reallocate instruction array
            numChunks++;
            Instruction **newInstructions = realloc(instructions, sizeof(Instruction *) * (CHUNK_SIZE * numChunks));

            // Check realloc result
            if (newInstructions == nullptr) {
                // Free old array
                free(instructions);

                // Report error and exit
                print_error(ERR_MALLOC_FAIL, true);
            }

            // Update instruction array
            instructions = newInstructions;
        }

        // Get instruction
        Instruction *inst = get_inst(reader, instCount);

        // Add to list
        instructions[instCount++] = inst;

        // EOF check
        if (inst->type == INST_EOF) {
            // Exit loop
            break;
        }
    }

    // Check if instruction count is requested
    if (numInstructions != nullptr) {
        // Save token count
        *numInstructions = instCount;
    }

    // Return instruction list
    return instructions;
}