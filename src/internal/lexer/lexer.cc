// Copyright 2024 Justin Cruz
#include "src/internal/lexer/lexer.h"

#include <stdlib.h>

#include <deque>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "re2/re2.h"

namespace lexer {
std::string Token::to_string() {
  switch (type) {
    case TokenType::Identifier:
      return "[IDENTIFIER=" + value + "]";
    case TokenType::Constant:
      return "[CONSTANT=" + value + "]";
    case TokenType::Int:
      return "[INT]";
    case TokenType::Void:
      return "[VOID]";
    case TokenType::Return:
      return "[RETURN]";
    case TokenType::OpenParen:
      return "[OPEN_PAREN]";
    case TokenType::ClosedParen:
      return "[CLOSED_PAREN]";
    case TokenType::OpenBrace:
      return "[OPEN_BRACE]";
    case TokenType::ClosedBrace:
      return "[CLOSED_BRACE]";
    case TokenType::Semicolon:
      return "[SEMICOLON]";
    default:
      return "[INVALID]";
  }
}

void PrintTokens(std::deque<Token>* token_list) {
  for (auto& token : (*token_list)) {
    std::cout << token.to_string();
  }

  std::cout << "\n";
}

std::deque<Token> ParseSourceFile(std::ifstream& source_file) {
  Search IdentifierSearch = Search("^([a-zA-Z_]\\w*\\b)", TokenType::Identifier);
  Search IntSearch = Search("^(int\\b)", TokenType::Int);
  Search ReturnSearch = Search("^(return\\b)", TokenType::Return);
  Search VoidSearch = Search("^(void\\b)", TokenType::Void);

  Search ConstantSearch = Search("^([0-9]+\\b)", TokenType::Constant);
  Search OpenParenSearch = Search("^(\\()", TokenType::OpenParen);
  Search ClosedParenSearch = Search("^(\\))", TokenType::ClosedParen);
  Search OpenBraceSearch = Search("^({)", TokenType::OpenBrace);
  Search ClosedBraceSearch = Search("^(})", TokenType::ClosedBrace);
  Search SemicolonSearch = Search("^(;)", TokenType::Semicolon);

  std::vector<Search> word_like_search = {
      IntSearch,
      VoidSearch,
      ReturnSearch,
  };

  std::vector<Search> non_word_like_search = {
      ConstantSearch,  OpenParenSearch,   ClosedParenSearch,
      OpenBraceSearch, ClosedBraceSearch, SemicolonSearch,
  };

  std::string source_line;
  std::deque<Token> token_list;
  int line_count = 1;
  while (std::getline(source_file, source_line)) {
    int line_length = source_line.length();
    while (source_line.length() > 0) {
      std::string identifier_match;
      std::string word_like_match;
      std::string non_word_like_match;

      source_line.erase(0, source_line.find_first_not_of(" \t\n\r\f\v"));

      if (re2::RE2::PartialMatch(source_line, IdentifierSearch.regex, &identifier_match)) {
        for (auto& word_like : word_like_search) {
          if (re2::RE2::PartialMatch(source_line, word_like.regex, &word_like_match)) {
            if (identifier_match == word_like_match) {
              token_list.push_back(Token(word_like.type, word_like_match));
              source_line = source_line.substr(word_like_match.length());
              break;
            }
          }
        }

        if (word_like_match.length() == 0) {
          token_list.push_back(Token(IdentifierSearch.type, identifier_match));
          source_line = source_line.substr(identifier_match.length());
        }
      } else {
        for (auto& non_word_like : non_word_like_search) {
          if (re2::RE2::PartialMatch(source_line, non_word_like.regex, &non_word_like_match)) {
            token_list.push_back(Token(non_word_like.type, non_word_like_match));
            source_line = source_line.substr(non_word_like_match.length());
            break;
          }
        }

        if (non_word_like_match.length() == 0) {
          throw std::invalid_argument(
              "Can't parse token: " + source_line + " Line: " + std::to_string(line_count) +
              " Col: " + std::to_string(line_length - source_line.length() + 1));
        }
      }
    }
    line_count++;
  }
  return token_list;
}
}  // namespace lexer
