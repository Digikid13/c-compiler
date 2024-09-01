// Copyright 2024 Justin Cruz
#include "assembler.h"

#include <stdlib.h>

#include <stdexcept>
#include <string>
#include <variant>

#include "src/internal/codegen/codegen.h"

namespace assembler {
std::string ParseOperand(codegen::Operand operand) {
  if (codegen::ImmediateOperand* imm_operand =
          std::get_if<codegen::ImmediateOperand>(&operand)) {
    return "$" + std::to_string(imm_operand->value);
  } else if (codegen::RegisterOperand* reg_operand =
                 std::get_if<codegen::RegisterOperand>(&operand)) {
    return "%" + reg_operand->reg;
  }

  throw std::invalid_argument("Attempted to parse invalid Operand");
}

std::string ParseInstruction(codegen::Instruction instruction) {
  if (codegen::MovInstruction* mov_instruct =
          std::get_if<codegen::MovInstruction>(&instruction)) {
    return "  movl " + ParseOperand(mov_instruct->source) + ", " +
           ParseOperand(mov_instruct->destination) + "\n";
  } else if (codegen::RetInstruction* ret_instruct =
                 std::get_if<codegen::RetInstruction>(&instruction)) {
    return "  ret\n";
  }

  throw std::invalid_argument("Attempted to parse invalid Instruction");
}

std::string ParseFunction(codegen::Function codegen_function) {
  std::string fn_name = codegen_function.identifier.value;
  std::string fn_output = "    .globl " + fn_name + "\n";
  fn_output += fn_name + ":\n";
  for (auto& instruction : codegen_function.instructions) {
    fn_output += ParseInstruction(instruction);
  }

  return fn_output;
}

std::string ParseProgram(codegen::Program codegen_program) {
  std::string function = ParseFunction(codegen_program.function);
  return function + "  .section .note.GUN-stack,\"\",@progbits\n";
}
}  // namespace assembler