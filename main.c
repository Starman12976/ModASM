/**
*  @file   main.c
 *  @brief  Compiles the ModASM script into machine code and builds the executable.
 *  @author Joel Nelems
 *  @date   2026-3-9
 *
 *  @details
 *  TODO
 *
 *  @example
 *  user:~$ ModASM file.asm -> file.exe
 */

// Include libraries
#include <stdio.h>
#include <stdlib.h>
#include "Scripts/Error/error.h"
#include "Scripts/Instruction/InstructionGen/instructionGen.h"
#include "Scripts/Parsing/Lexer/lexer.h"
#include "Scripts/Tokenization/Tokenizer/tokenizer.h"
#include "Scripts/Tokenization/TokenReader/tokenReader.h"
#include "Scripts/Instruction/InstructionReader/instructionReader.h"

/**
 *  @fn     main
 *  @brief  Compiles a provided ModASM script into machine code and builds the executable.
 *
 *  @param  [in] argc Number of program arguments.
 *  @param  [in] argv Program arguments.
 *  @return Exit code.
 */
int main(const size_t argc, const char *const *const argv) {
    // Verify argument count
    if (argc != 2) {
        // Report error and exit
        print_error(ERR_WRONG_ARGC, true);
    }

    // Open file
    FILE *const fp = fopen(argv[1], "rb");

    // Ensure file opened successfully
    if (fp == nullptr) {
        // Report error
        print_error(ERR_CANNOT_OPEN, argv[1]);

        // Exit program
        exit(ERR_CANNOT_OPEN);
    }

    // Create lexer
    Lexer *const lexer = lexer_init(fp);

    // Print text
    lexer->print(lexer);
    printf("\n");

    // Convert script to tokens
    size_t numTokens;
    const Token *const *const tokens = get_tokens(lexer, &numTokens);

    // Create token reader
    const TokenReader *const token_reader = token_reader_init(lexer);

    // Create instruction reader
    InstructionReader *const instructionReader = instruction_reader_init(token_reader);

    // Print instructions
    instructionReader->print(instructionReader);

    // Return successfully
    return 0;
}