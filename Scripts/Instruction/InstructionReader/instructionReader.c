/**
 *  @file   instructionReader.c
 *  @brief  Reads through instructions.
 *  @author Joel Nelems
 *  @date   2026-3-9
 *
 *  @details
 *  TODO
 */

// Include libraries
#include <stdlib.h>
#include "instructionReader.h"
#include "../Instruction/instruction.h"
#include "../InstructionGen/instructionGen.h"
#include "../../Error/error.h"

// Declare functions
static const Instruction  *peek                   (const InstructionReader *, long long);
static const Instruction  *advance                (InstructionReader *);
static const Instruction **get_instruction_list   (const InstructionReader *, size_t, size_t);
static size_t              get_bound              (const InstructionReader *, long long);
static void                reset                  (InstructionReader *);
static void                print                  (const InstructionReader *);
static bool                ended                  (const InstructionReader *);
static void                free_instruction_reader(InstructionReader *);

/**
 *  @var   instTypeStrs
 *  @brief Allows writing instruction types as strings for printing.
 */
const char *const instTypeStrs[INST_COUNT] = {
    // Special
    [INST_EOF]   = "EOF",
    [INST_EMPTY] = "EMPTY",
    [INST_ERROR] = "ERROR",

    // Macros
    [INST_REPLACE] = "REPLACE",

    // Declarations
    [INST_DEFINE] = "DEFINE",
    [INST_LET]    = "LET",

    // Movement
    [INST_MOVE] = "MOVE",

    // Calling
    [INST_SYSCALL] = "SYSCALL",
    [INST_JUMP]    = "JUMP",
    [INST_RETURN]  = "RETURN",
};

/**
 *  @fn    instruction_reader_init
 *  @brief TODO
 *
 *  @param  [in, out] tokenReader TODO
 *  @return TODO
 */
InstructionReader *instruction_reader_init(TokenReader *const tokenReader) {
    // Verify token reader
    if (tokenReader == nullptr) {
        // Report error and exit
        print_error(ERR_INVALID_TOKEN_READER, true);
    }

    // Create new instruction reader
    InstructionReader *self = malloc(sizeof(InstructionReader));

    // Verify malloc result
    if (self == nullptr) {
        // Report error and exit
        print_error(ERR_MALLOC_FAIL, true);
    }

    // Initialize instruction reader members
    self->instructions = get_instructions(tokenReader, &self->numInstructions);
    self->offset       = 0;
    self->line         = self->instructions[0]->line;
    self->column       = self->instructions[0]->column;

    // Set instruction reader methods
    self->peek                 = peek;
    self->advance              = advance;
    self->get_instruction_list = get_instruction_list;
    self->get_bound            = get_bound;
    self->print                = print;
    self->reset                = reset;
    self->ended                = ended;
    self->free                 = free_instruction_reader;

    // Return instruction reader
    return self;
}

/**
 *  @fn    peek
 *  @brief TODO
 *
 *  @param  [in] self   TODO
 *  @param  [in] offset TODO
 *  @return TODO
 */
static const Instruction *peek(const InstructionReader *const self, const long long offset) {
    // Check for valid instruction reader
    if (self == nullptr) {
        // Report error and exit
        print_error(ERR_INVALID_INST_READER, true);
    }

    // Get bounded offset
    const size_t boundedOffset = self->get_bound(self, offset);

    // Return token at given offset
    return self->instructions[self->offset + boundedOffset];
}

/**
 *  @fn    advance
 *  @brief TODO
 *
 *  @param  [in, out] self TODO
 *  @return TODO
 */
static const Instruction *advance(InstructionReader *const self) {
    // Check for valid instruction reader
    if (self == nullptr) {
        // Report error and exit
        print_error(ERR_INVALID_INST_READER, true);
    }

    // Check if end has been reached
    if (self->ended(self)) {
        // Return last token
        return self->get_last(self);
    }

    // Get current instruction and advance
    const Instruction *const inst = self->instructions[self->offset++];

    // Update reader positions
    self->line   = inst->line;
    self->column = inst->column;

    // Return instruction
    return inst;
}

/**
 *  @fn    get_instruction_list
 *  @brief TODO
 *
 *  @param  [in] self      TODO
 *  @param  [in] start          TODO
 *  @param  [in] numInstructions TODO
 *  @return TODO
 */
static const Instruction **get_instruction_list(const InstructionReader *const self, const size_t start, size_t numInstructions) {
    // Check for valid instruction reader
    if (self == nullptr) {
        // Report error and exit
        print_error(ERR_INVALID_INST_READER, true);
    }

    // Check bounds
    if (start >= self->numInstructions) {
        // Return empty list
        Instruction **const instructions = malloc(0);
        return instructions;
    }

    // Check token count
    if (start + numInstructions >= self->numInstructions) {
        // Clamp instruction count
        numInstructions = self->numInstructions - start;
    }

    // Initialize instruction list
    const Instruction **const instructions = malloc(sizeof(Instruction *) * numInstructions);

    // Loop through indices
    for (size_t instIndex = 0; instIndex < numInstructions; instIndex++) {
        // Add instruction to list
        instructions[instIndex] = self->instructions[start + instIndex];
    }

    // Return instruction list
    return instructions;
}

/**
 *  @fn    get_bound
 *  @brief TODO
 *
 *  @param  [in] self   TODO
 *  @param  [in] offset TODO
 *  @return TODO
 */
static size_t get_bound(const InstructionReader *const self, const long long offset) {
    // Check for valid instruction reader
    if (self == nullptr) {
        // Report error and exit
        print_error(ERR_INVALID_INST_READER, true);
    }

    // Verify upper bound
    if (self->offset + offset >= self->numInstructions) {
        // Return last token index
        return self->numInstructions - self->offset;
    }

    // Verify lower bound
    if (self->offset + offset < 0) {
        // Return first index
        return -self->offset;
    }

    // Return valid offset
    return offset;
}

/**
 *  @fn    get_last
 *  @brief TODO
 *
 *  @param  [in] self TODO
 *  @return TODO
 */
static const Instruction *get_last(const InstructionReader *const self) {
    // Check for valid instruction reader
    if (self == nullptr) {
        // Report error and exit
        print_error(ERR_INVALID_INST_READER, true);
    }

    // Return last token
    return self->instructions[self->numInstructions - 1];
}

/**
 *  @fn    ended
 *  @brief TODO
 *
 *  @param  [in] self TODO
 *  @return TODO
 */
static bool ended(const InstructionReader *const self) {
    // Check for valid instruction reader
    if (self == nullptr) {
        // Report error and exit
        print_error(ERR_INVALID_INST_READER, true);
    }

    // Check if offset is at end
    if (self->offset >= self->numInstructions - 1) {
        // Return true
        return true;
    }

    // Return false
    return false;
}

/**
 *  @fn    reset
 *  @brief TODO
 *
 *  @param  [in] self TODO
 *  @return Void.
 */
static void reset(InstructionReader *self) {
    // Check for valid instruction reader
    if (self == nullptr) {
        // Report error and exit
        print_error(ERR_INVALID_INST_READER, true);
    }

    // Reset position
    self->offset = 0;
    self->line   = self->instructions[0]->line;
    self->column = self->instructions[0]->column;
}

/**
 *  @fn    print
 *  @brief TODO
 *
 *  @param  [in] self TODO
 *  @return Void.
 */
static void print(const InstructionReader *const self) {
    // Check for valid instruction reader
    if (self == nullptr) {
        // Report error and exit
        print_error(ERR_INVALID_INST_READER, true);
    }

    // Loop through instructions
    for (const Instruction *const *instP = self->instructions; (*instP)->type != INST_EOF; instP++) {
        // Skip empty instructions
        if ((*instP)->type == INST_EMPTY) {
            continue;
        }

        // Get instruction as string
        const char *instStr = instTypeStrs[(*instP)->type] != nullptr ? instTypeStrs[(*instP)->type] : "Undefined";

        // Print instruction info
        printf("Index: %-3llu | Type: %-14s | Tokens: ",
            (*instP)->index, instStr);

        // Print tokens
        for (size_t tokIndex = 0; tokIndex < (*instP)->numTokens; tokIndex++) {
            // Ignore empty tokens
            if ((*instP)->tokens[tokIndex]->type == TOK_EOI) {
                // Continue loop
                continue;
            }

            // Print token
            printf("%s ", (*instP)->tokens[tokIndex]->lexeme);
        }

        // Print newline
        printf("\n");
    }
}

/**
 *  @fn    free_instruction_reader
 *  @brief TODO
 *
 *  @param  [out] self TODO
 *  @return Void.
 */
static void free_instruction_reader(InstructionReader *const self) {
    // Check for valid instruction reader
    if (self == nullptr) {
        // Report error and exit
        print_error(ERR_INVALID_INST_READER, true);
    }

    // Free instruction reader members
    for (size_t instIndex = 0; instIndex < self->numInstructions; instIndex++) {
        // Free instructions
        self->instructions[instIndex]->free(self->instructions[instIndex]);
    }

    // Free instruction reader
    free(self);
}