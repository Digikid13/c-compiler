// Copyright 2024 Justin Cruz
#ifndef C_COMPILER_SRC_ASSEMBLER_H_
#define C_COMPILER_SRC_ASSEMBLER_H_

#include <stdlib.h>

#include <string>

#include "src/internal/codegen/codegen.h"

namespace assembler {
std::string ParseOperand(codegen::Operand operand);

std::string ParseInstruction(codegen::Instruction instruction);

std::string ParseFunction(codegen::Function codegen_function);

std::string ParseProgram(codegen::Program codegen_program);
}  // namespace assembler

#endif  // C_COMPILER_SRC_ASSEMBLER_H_
