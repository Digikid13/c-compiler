// Copyright 2024 Justin Cruz
#include "src/internal/parser/parser.h"

#include <stdlib.h>

#include <deque>
#include <stdexcept>
#include <string>
#include <variant>

#include "src/internal/lexer/lexer.h"

namespace parser {
void ExpectToken(lexer::Token token, lexer::TokenType token_type,
                 std::string expected) {
  if (token.type != token_type) {
    throw std::invalid_argument("Expected " + expected +
                                " found :" + token.to_string());
  }
}

Expression ParseExpression(std::deque<lexer::Token>* token_list) {
  lexer::Token return_token = (*token_list).front();
  ExpectToken(return_token, lexer::TokenType::Return, "return");
  (*token_list).pop_front();

  lexer::Token constant_token = (*token_list).front();
  ExpectToken(constant_token, lexer::TokenType::Constant, "a constant value");
  (*token_list).pop_front();

  return Expression(ExpressionType::Return, Constant(constant_token.value));
}

Statement ParseStatement(std::deque<lexer::Token>* token_list) {
  Statement statement_node = Statement(ParseExpression(token_list));

  lexer::Token semicolon_token = (*token_list).front();
  ExpectToken(semicolon_token, lexer::TokenType::Semicolon, ";");
  (*token_list).pop_front();

  return statement_node;
}

Function ParseFunction(std::deque<lexer::Token>* token_list) {
  lexer::Token type_token = (*token_list).front();
  ExpectToken(type_token, lexer::TokenType::Int, "a type");
  (*token_list).pop_front();

  lexer::Token identifier_token = (*token_list).front();
  ExpectToken(identifier_token, lexer::TokenType::Identifier, "an identifier");
  (*token_list).pop_front();

  lexer::Token open_paren_token = (*token_list).front();
  ExpectToken(open_paren_token, lexer::TokenType::OpenParen, "(");
  (*token_list).pop_front();

  lexer::Token void_token = (*token_list).front();
  ExpectToken(void_token, lexer::TokenType::Void, "void");
  (*token_list).pop_front();

  lexer::Token closed_paren_token = (*token_list).front();
  ExpectToken(closed_paren_token, lexer::TokenType::ClosedParen, ")");
  (*token_list).pop_front();

  lexer::Token open_brace_token = (*token_list).front();
  ExpectToken(open_brace_token, lexer::TokenType::OpenBrace, "open_brace");
  (*token_list).pop_front();

  Function function_node =
      Function(Identifier(identifier_token.value), ParseStatement(token_list));

  lexer::Token closed_brace_token = (*token_list).front();
  ExpectToken(closed_brace_token, lexer::TokenType::ClosedBrace,
              "closed_brace");
  (*token_list).pop_front();

  return function_node;
}

Program ParseProgram(std::deque<lexer::Token>* token_list) {
  Program program_node = Program(ParseFunction(token_list));

  if ((*token_list).size() > 0) {
    throw std::logic_error("Parse failed to consume all Lexer tokens.");
  }

  return program_node;
}
}  // namespace parser
