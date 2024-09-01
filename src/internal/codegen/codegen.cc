// Copyright 2024 Justin Cruz
#include "codegen.h"

#include <stdlib.h>

#include <deque>
#include <string>
#include <variant>

#include "src/internal/parser/parser.h"

namespace codegen {
std::deque<Instruction> ParseInstructions(parser::Statement parser_statement) {
  std::deque<Instruction> instruction_list;
  if (parser_statement.expression.type == parser::ExpressionType::Return) {
    ImmediateOperand src_operand = ImmediateOperand(std::stoi(
        std::get<parser::Constant>(parser_statement.expression.value).value));
    RegisterOperand dest_operand = RegisterOperand();

    MovInstruction mov_instruct = MovInstruction(src_operand, dest_operand);
    RetInstruction ret_instruct = RetInstruction();

    instruction_list.push_back(mov_instruct);
    instruction_list.push_back(ret_instruct);
  }

  return instruction_list;
}

Function ParseFunction(parser::Function parser_function) {
  Identifier identifier = Identifier(parser_function.identifier.value);
  Function function =
      Function(identifier, ParseInstructions(parser_function.statement));

  return function;
}

Program ParseProgram(parser::Program parser_program) {
  Program program = Program(ParseFunction(parser_program.function));

  return program;
}
}  // namespace codegen
