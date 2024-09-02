// Copyright 2024 Justin Cruz
#ifndef C_COMPILER_SRC_INTERNAL_LEXER_LEXER_H_
#define C_COMPILER_SRC_INTERNAL_LEXER_LEXER_H_

#include <stdlib.h>

#include <deque>
#include <fstream>
#include <string>
#include <vector>

#include "re2/re2.h"

namespace lexer {
enum class TokenType {
  // Word-like Tokens
  Identifier,
  Int,
  Return,
  Void,

  // Non-word-like Tokens
  OpenBrace,
  ClosedBrace,
  Bitwise,
  Constant,
  Decrement,
  Negation,
  OpenParen,
  ClosedParen,
  Semicolon
};

struct Search {
  std::string regex;
  TokenType type;

  Search(std::string r, TokenType t) : regex(r), type(t) {}
};

struct Token {
  TokenType type;
  std::string value;

  explicit Token(TokenType t) : type(t), value{} {}
  Token(TokenType t, std::string v) : type(t), value(v) {}

  std::string to_string();
};

void PrintTokens(std::deque<Token>* token_list);

std::deque<Token> ParseSourceFile(std::ifstream& source_file);
}  // namespace lexer

#endif  // C_COMPILER_SRC_INTERNAL_LEXER_LEXER_H_
