// Copyright 2024 Justin Cruz
#ifndef C_COMPILER_SRC_LEXER_H_
#define C_COMPILER_SRC_LEXER_H_

#include <stdlib.h>

#include <deque>
#include <fstream>
#include <string>

namespace lexer {
enum class TokenType {
  Invalid,
  Identifier,
  Constant,
  Int,
  Void,
  Return,
  OpenParen,
  ClosedParen,
  OpenBrace,
  ClosedBrace,
  Semicolon
};

struct Token {
  TokenType type;
  std::string value;

  Token(TokenType t) : type(t), value{} {}
  Token(TokenType t, std::string v) : type(t), value(v) {}

  std::string to_string();
};

void PrintTokens(std::deque<Token>* token_list);

std::deque<Token> ParseSourceFile(std::ifstream& source_file);
}  // namespace lexer

#endif  // C_COMPILER_SRC_LEXER_H_
