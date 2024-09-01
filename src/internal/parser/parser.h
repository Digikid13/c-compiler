// Copyright 2024 Justin Cruz
#ifndef C_COMPILER_SRC_PARSER_H_
#define C_COMPILER_SRC_PARSER_H_

#include <stdlib.h>

#include <deque>
#include <string>
#include <variant>

#include "src/internal/lexer/lexer.h"

namespace parser {
struct Identifier {
  std::string value;

  Identifier(std::string v) : value(v) {}

  std::string to_string() { return "Identifier { value: " + value + " }"; }
};

struct Constant {
  std::string value;

  Constant(std::string v) : value(v) {}

  std::string to_string() { return "Constant { value: " + value + " }"; }
};

enum ExpressionType {
  Return,
};

struct Expression {
  ExpressionType type;
  std::variant<Constant> value;

  Expression(ExpressionType t, std::variant<Constant> c) : type(t), value(c) {}

  std::string to_string() {
    std::string type_string;
    switch (type) {
      case ExpressionType::Return:
        type_string = "Return";
        break;
      default:
        type_string = "";
        break;
    }

    std::string value_string;
    if (auto* constant = std::get_if<Constant>(&value)) {
      value_string = constant->to_string();
    }
    return "Expression {\n        type: " + type_string +
           "\n        value: " + value_string + "\n      }";
  }
};

struct Statement {
  Expression expression;

  Statement(Expression e) : expression(e) {}

  std::string to_string() {
    return "Statement {\n      expression: " + expression.to_string() +
           "\n    }";
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

  Program(Function f) : function(f) {}

  std::string to_string() {
    return "Program {\n  " + function.to_string() + "\n}\n";
  }
};

void ExpectToken(lexer::Token token, lexer::TokenType token_type,
                 std::string expected);

Expression ParseExpression(std::deque<lexer::Token>* token_list);

Statement ParseStatement(std::deque<lexer::Token>* token_list);

Function ParseFunction(std::deque<lexer::Token>* token_list);

Program ParseProgram(std::deque<lexer::Token>* token_list);
}  // namespace parser

#endif  // C_COMPILER_SRC_PARSER_H_
