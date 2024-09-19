// Copyright 2024 Justin Cruz
#include "src/internal/lexer/lexer.h"

#include <gtest/gtest.h>

TEST(LexerTest, ValidateTokens) {
  // Word-like Tokens
  lexer::Token identifier_token(lexer::TokenType::Identifier, "a_name");
  lexer::Token int_token(lexer::TokenType::Int);
  lexer::Token return_token(lexer::TokenType::Return);
  lexer::Token void_token(lexer::TokenType::Void);

  EXPECT_EQ(identifier_token.to_string(), "[IDENTIFIER=a_name]");
  EXPECT_EQ(int_token.to_string(), "[INT]");
  EXPECT_EQ(return_token.to_string(), "[RETURN]");
  EXPECT_EQ(void_token.to_string(), "[VOID]");

  // Non-word-like Tokens
  lexer::Token open_brace_token(lexer::TokenType::OpenBrace);
  lexer::Token closed_brace_token(lexer::TokenType::ClosedBrace);
  lexer::Token complement_token(lexer::TokenType::Complement);
  lexer::Token constant_token(lexer::TokenType::Constant, "42");
  lexer::Token decrement_token(lexer::TokenType::Decrement);
  lexer::Token negation_token(lexer::TokenType::Negation);
  lexer::Token open_paren_token(lexer::TokenType::OpenParen);
  lexer::Token closed_paren_token(lexer::TokenType::ClosedParen);
  lexer::Token semicolon_token(lexer::TokenType::Semicolon);

  EXPECT_EQ(open_brace_token.to_string(), "[OPEN_BRACE]");
  EXPECT_EQ(closed_brace_token.to_string(), "[CLOSED_BRACE]");
  EXPECT_EQ(complement_token.to_string(), "[COMPLEMENT]");
  EXPECT_EQ(constant_token.to_string(), "[CONSTANT=42]");
  EXPECT_EQ(decrement_token.to_string(), "[DECREMENT]");
  EXPECT_EQ(negation_token.to_string(), "[NEGATION]");
  EXPECT_EQ(open_paren_token.to_string(), "[OPEN_PAREN]");
  EXPECT_EQ(closed_paren_token.to_string(), "[CLOSED_PAREN]");
  EXPECT_EQ(semicolon_token.to_string(), "[SEMICOLON]");
}
