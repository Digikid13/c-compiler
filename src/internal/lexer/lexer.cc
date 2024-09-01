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
    case TokenType::Invalid:
      return "[INVALID]";
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
  re2::RE2 identifierRegex("^([a-zA-Z_]\\w*\\b)");
  re2::RE2 constantRegex("^([0-9]+\\b)");
  re2::RE2 intRegex("^(int\\b)");
  re2::RE2 voidRegex("^(void\\b)");
  re2::RE2 returnRegex("^(return\\b)");
  re2::RE2 openParenRegex("^(\\()");
  re2::RE2 closedParenRegex("^(\\))");
  re2::RE2 openBraceRegex("^({)");
  re2::RE2 closedBraceRegex("^(})");
  re2::RE2 semicolonRegex("^(;)");

  std::string source_line;
  std::deque<Token> token_list;
  int line_count = 1;
  while (std::getline(source_file, source_line)) {
    int line_length = source_line.length();
    while (source_line.length() > 0) {
      source_line.erase(0, source_line.find_first_not_of(" \t\n\r\f\v"));

      std::string identifierMatch;
      std::string constantMatch;
      std::string intMatch;
      std::string voidMatch;
      std::string returnMatch;
      std::string openParenMatch;
      std::string closedParenMatch;
      std::string openBraceMatch;
      std::string closedBraceMatch;
      std::string semicolonMatch;

      re2::RE2::PartialMatch(source_line, identifierRegex, &identifierMatch);
      re2::RE2::PartialMatch(source_line, constantRegex, &constantMatch);
      re2::RE2::PartialMatch(source_line, intRegex, &intMatch);
      re2::RE2::PartialMatch(source_line, voidRegex, &voidMatch);
      re2::RE2::PartialMatch(source_line, returnRegex, &returnMatch);
      re2::RE2::PartialMatch(source_line, openParenRegex, &openParenMatch);
      re2::RE2::PartialMatch(source_line, closedParenRegex, &closedParenMatch);
      re2::RE2::PartialMatch(source_line, openBraceRegex, &openBraceMatch);
      re2::RE2::PartialMatch(source_line, closedBraceRegex, &closedBraceMatch);
      re2::RE2::PartialMatch(source_line, semicolonRegex, &semicolonMatch);

      // std::cout << identifierMatch << "\n";
      // std::cout << constantMatch << "\n";
      // std::cout << intMatch << "\n";
      // std::cout << voidMatch << "\n";
      // std::cout << returnMatch << "\n";
      // std::cout << openParenMatch << "\n";
      // std::cout << closedParenMatch << "\n";
      // std::cout << openBraceMatch << "\n";
      // std::cout << closedBraceMatch << "\n";
      // std::cout << semicolonMatch << "\n";

      if (identifierMatch.length() > 0) {
        if (intMatch.length() > 0 && identifierMatch == intMatch) {
          token_list.push_back(Token(TokenType::Int));
          source_line = source_line.substr(3);
        } else if (voidMatch.length() > 0 && identifierMatch == voidMatch) {
          token_list.push_back(Token(TokenType::Void));
          source_line = source_line.substr(4);
        } else if (returnMatch.length() > 0 && identifierMatch == returnMatch) {
          token_list.push_back(Token(TokenType::Return));
          source_line = source_line.substr(6);
        } else {
          token_list.push_back(Token(TokenType::Identifier, identifierMatch));
          source_line = source_line.substr(identifierMatch.length());
        }
      } else if (intMatch.length() > 0) {
        token_list.push_back(Token(TokenType::Int));
        source_line = source_line.substr(3);
      } else if (voidMatch.length() > 0) {
        token_list.push_back(Token(TokenType::Void));
        source_line = source_line.substr(4);
      } else if (returnMatch.length() > 0) {
        token_list.push_back(Token(TokenType::Return));
        source_line = source_line.substr(6);
      } else if (constantMatch.length() > 0) {
        token_list.push_back(Token(TokenType::Constant, constantMatch));
        source_line = source_line.substr(constantMatch.length());
      } else if (openParenMatch.length() > 0) {
        token_list.push_back(Token(TokenType::OpenParen));
        source_line = source_line.substr(1);
      } else if (closedParenMatch.length() > 0) {
        token_list.push_back(Token(TokenType::ClosedParen));
        source_line = source_line.substr(1);
      } else if (openBraceMatch.length() > 0) {
        token_list.push_back(Token(TokenType::OpenBrace));
        source_line = source_line.substr(1);
      } else if (closedBraceMatch.length() > 0) {
        token_list.push_back(Token(TokenType::ClosedBrace));
        source_line = source_line.substr(1);
      } else if (semicolonMatch.length() > 0) {
        token_list.push_back(Token(TokenType::Semicolon));
        source_line = source_line.substr(1);
      } else {
        throw std::invalid_argument(
            "Can't parse token: " + source_line +
            " Line: " + std::to_string(line_count) +
            " Col: " + std::to_string(line_length - source_line.length() + 1));
      }
    }
    line_count++;
  }
  return token_list;
}
}  // namespace lexer
