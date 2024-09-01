// Copyright 2024 Justin Cruz
#ifndef C_COMPILER_SRC_INTERNAL_CODEGEN_CODEGEN_H_
#define C_COMPILER_SRC_INTERNAL_CODEGEN_CODEGEN_H_

#include <stdlib.h>

#include <deque>
#include <string>
#include <variant>

#include "src/internal/parser/parser.h"

namespace codegen {
struct Identifier {
  std::string value;

  explicit Identifier(std::string v) : value(v) {}

  std::string to_string() { return "Identifier { value: " + value + " }"; }
};

struct ImmediateOperand {
  int value;

  explicit ImmediateOperand(int v) : value(v) {}

  std::string to_string() {
    return "ImmediateOperand { value : " + std::to_string(value) + " }";
  }
};

struct RegisterOperand {
  std::string reg = "eax";

  RegisterOperand() : reg("eax") {}
  explicit RegisterOperand(std::string r) : reg(r) {}

  std::string to_string() { return "ImmediateOperand { value : " + reg + " }"; }
};

using Operand = std::variant<ImmediateOperand, RegisterOperand>;

struct MovInstruction {
  Operand source;
  Operand destination;

  MovInstruction(Operand s, Operand d) : source(s), destination(d) {}
};

struct RetInstruction {};

using Instruction = std::variant<MovInstruction, RetInstruction>;

struct Function {
  // ReturnType return_type;
  Identifier identifier;
  std::deque<Instruction> instructions;

  Function(Identifier id, std::deque<Instruction> in)
      : identifier(id), instructions(in) {}

  std::string to_string() {
    return "Function {\n    identifier: " +
           identifier.to_string();  // +
                                    // "\n    instructions: " +
                                    // instructions.to_string() + "\n  }";
  }
};

struct Program {
  Function function;

  explicit Program(Function f) : function(f) {}

  std::string to_string() {
    return "Program {\n  " + function.to_string() + "\n}\n";
  }
};

std::deque<Instruction> ParseInstructions(parser::Statement parser_statement);

Function ParseFunction(parser::Function parser_function);

Program ParseProgram(parser::Program parser_program);
}  // namespace codegen

#endif  // C_COMPILER_SRC_INTERNAL_CODEGEN_CODEGEN_H_
