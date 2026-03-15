/**
 *  @file   instructionGen.h
 *  @brief  Uses a Token Reader to generate an instruction list.
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
#ifndef INSTRUCTIONGEN_H
#define INSTRUCTIONGEN_H

// Include libraries
#include "../Instruction/instruction.h"
#include "../../Tokenization/TokenReader/tokenReader.h"

Instruction **get_instructions(TokenReader *, size_t *);

// Include guard
#endif //INSTRUCTIONGEN_H
