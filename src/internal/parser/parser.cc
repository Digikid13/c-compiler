// Copyright 2024 Justin Cruz
#include "src/internal/parser/parser.h"

#include <stdlib.h>

#include <deque>
#include <stdexcept>
#include <string>
#include <variant>

#include "src/internal/lexer/lexer.h"

namespace parser {
lexer::Token ExpectToken(std::deque<lexer::Token>* token_list, lexer::TokenType token_type,
                         std::string expected) {
  lexer::Token validate_token = token_list->front();
  if (validate_token.type != token_type) {
    throw std::invalid_argument("Expected " + expected + " found :" + validate_token.to_string());
  }
  token_list->pop_front();
  return validate_token;
}

Expression ParseExpression(std::deque<lexer::Token>* token_list) {
  lexer::Token constant_token =
      ExpectToken(token_list, lexer::TokenType::Constant, "a constant value");

  return ConstantExpression(constant_token.value);
}

Statement ParseStatement(std::deque<lexer::Token>* token_list) {
  ExpectToken(token_list, lexer::TokenType::Return, "return");
  Statement statement_node = Statement(ParseExpression(token_list));

  ExpectToken(token_list, lexer::TokenType::Semicolon, ";");

  return statement_node;
}

Function ParseFunction(std::deque<lexer::Token>* token_list) {
  ExpectToken(token_list, lexer::TokenType::Int, "a type");
  lexer::Token identifier_token =
      ExpectToken(token_list, lexer::TokenType::Identifier, "an identifier");
  ExpectToken(token_list, lexer::TokenType::OpenParen, "(");
  ExpectToken(token_list, lexer::TokenType::Void, "void");
  ExpectToken(token_list, lexer::TokenType::ClosedParen, ")");
  ExpectToken(token_list, lexer::TokenType::OpenBrace, "open_brace");

  Function function_node = Function(Identifier(identifier_token.value), ParseStatement(token_list));

  ExpectToken(token_list, lexer::TokenType::ClosedBrace, "closed_brace");

  return function_node;
}

Program ParseProgram(std::deque<lexer::Token>* token_list) {
  Program program_node = Program(ParseFunction(token_list));

  if (token_list->size() > 0) {
    throw std::logic_error("Parse failed to consume all Lexer tokens.");
  }

  return program_node;
}
}  // namespace parser
