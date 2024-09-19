// Copyright 2024 Justin Cruz
#ifndef C_COMPILER_SRC_INTERNAL_PARSER_PARSER_H_
#define C_COMPILER_SRC_INTERNAL_PARSER_PARSER_H_

#include <stdlib.h>

#include <deque>
#include <string>
#include <variant>

#include "src/internal/lexer/lexer.h"

namespace parser {
struct Identifier {
  std::string value;

  explicit Identifier(std::string v) : value(v) {}

  std::string to_string() { return "Identifier { value: " + value + " }"; }
};

struct ConstantExpression {
  std::string value;

  ConstantExpression() : value("NaN") {}
  explicit ConstantExpression(std::string v) : value(v) {}

  std::string to_string() { return "ConstantExpression { value: " + value + " }"; }
};

using UnaryExpression = struct UnaryExpression;
using Expression = std::variant<ConstantExpression, UnaryExpression>;

enum class UnaryOperator {
  Invalid,
  Complement,
  Negate,
};

struct UnaryExpression {
  UnaryOperator op;
  Expression* exp;

  UnaryExpression() : op(UnaryOperator::Invalid), exp(nullptr) {}
  UnaryExpression(UnaryOperator op, Expression* e) : op(op), exp(e) {}

  std::string to_string() { return "Unary Expression {}"; }
};

enum StatementType {
  Return,
};

struct Statement {
  StatementType type = StatementType::Return;
  Expression expression;

  explicit Statement(Expression e) : expression(e) {}

  std::string to_string() {
    if (std::holds_alternative<ConstantExpression>(expression)) {
      return "Statement {\n      expression: " +
             std::get<ConstantExpression>(expression).to_string() + "\n    }";
    } else if (std::holds_alternative<UnaryExpression>(expression)) {
      return "Statement {\n      expression: " +
      std::get<UnaryExpression>(expression).to_string() +
             "\n    }";
    }

    return "";
  }
};

struct Function {
  // ReturnType return_type;
  Identifier identifier;
  Statement statement;

  Function(Identifier i, Statement s) : identifier(i), statement(s) {}

  std::string to_string() {
    return "Function {\n    identifier: " + identifier.to_string() +
           "\n    statement: " + statement.to_string() + "\n  }";
  }
};

struct Program {
  Function function;

  explicit Program(Function f) : function(f) {}

  std::string to_string() { return "Program {\n  " + function.to_string() + "\n}\n"; }
};

lexer::Token ExpectToken(std::deque<lexer::Token>* token_list, lexer::TokenType token_type,
                         std::string expected);

Expression ParseExpression(std::deque<lexer::Token>* token_list);

Statement ParseStatement(std::deque<lexer::Token>* token_list);

Function ParseFunction(std::deque<lexer::Token>* token_list);

Program ParseProgram(std::deque<lexer::Token>* token_list);
}  // namespace parser

#endif  // C_COMPILER_SRC_INTERNAL_PARSER_PARSER_H_
