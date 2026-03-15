cmake_minimum_required(VERSION 3.30)
project(ModASM C)

set(CMAKE_C_STANDARD 23)

add_executable(ModASM main.c
        Scripts/Write/write.h
        Scripts/Write/write.c
        Scripts/Tokenization/TokenReader/tokenReader.h
        Scripts/Tokenization/TokenReader/tokenReader.c
        Scripts/Tokenization/Tokenizer/tokenizer.h
        Scripts/Tokenization/Tokenizer/tokenizer.c
        Scripts/Tokenization/Token/token.h
        Scripts/Tokenization/Token/token.c
        Scripts/Parsing/Lexer/lexer.h
        Scripts/Parsing/Lexer/lexer.c
        Scripts/Parsing/File/file.h
        Scripts/Parsing/File/file.c
        Scripts/Instruction/InstructionReader/instructionReader.h
        Scripts/Instruction/InstructionReader/instructionReader.c
        Scripts/Instruction/InstructionGen/instructionGen.h
        Scripts/Instruction/InstructionGen/instructionGen.c
        Scripts/Instruction/Instruction/instruction.h
        Scripts/Instruction/Instruction/instruction.c
        Scripts/Error/error.h
        Scripts/Error/error.c
        Scripts/Build/build.h
        Scripts/Build/build.c
        Scripts/Build/ELF/elf.h
        Scripts/Build/ELF/elf.c)
