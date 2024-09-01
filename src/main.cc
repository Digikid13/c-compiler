// Copyright 2024 Justin Cruz
#include <stdlib.h>

#include <deque>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>

#include "src/internal/assembler/assembler.h"
#include "src/internal/codegen/codegen.h"
#include "src/internal/lexer/lexer.h"
#include "src/internal/parser/parser.h"

namespace file_utils {
bool IsValidFilename(const std::string& file_name) {
  return file_name.substr(file_name.length() - 2) == ".c";
}
}  // namespace file_utils

int main(int argc, char* argv[]) {
  if (argc < 2 || argc > 3 || (argc == 3 && argv[1][0] != '-')) {
    std::cerr << "Usage: " << argv[0]
              << " [--lex|--parse|--codegen] <file_name>\n";
    std::cerr << "  --lex " << " Run only the lexer\n";
    std::cerr << "  --parse " << " Run the lexer and parser\n";
    std::cerr << "  --codegen "
              << " Run the lexer, parser, and assembly generation \n";
    return 1;
  }

  std::string source_file_name;
  std::string flag;

  if (argc == 2) {
    source_file_name = argv[1];
  } else if (argc == 3) {
    flag = argv[1];
    source_file_name = argv[2];
  }

  if (!file_utils::IsValidFilename(source_file_name)) {
    std::cerr << "File name is not valid: " << source_file_name
              << " should end in *.c\n";
    return 1;
  }

  std::ifstream source_file(source_file_name.c_str());

  if (!source_file) {
    std::cerr << "Failed to load file: " << source_file_name;
    return 1;
  }

  source_file.close();

  std::string file_prefix =
      source_file_name.substr(0, source_file_name.length() - 2);
  std::string preprocessed_file_name = file_prefix + ".i";
  std::string preprocess_cmd =
      "gcc -E -P " + source_file_name + " -o " + preprocessed_file_name;
  // std::cout << preprocess_cmd << "\n";

  int preprocess_ret = system(preprocess_cmd.c_str());

  if (preprocess_ret != 0) {
    std::cerr << "Something went wrong preprocessing the file "
              << source_file_name << "\n";
  }

  std::ifstream preprocessed_file(preprocessed_file_name.c_str());

  if (!preprocessed_file) {
    std::cerr << "Failed to load file: " << preprocessed_file_name;
    return 1;
  }

  std::deque<lexer::Token> token_list;

  try {
    token_list = lexer::ParseSourceFile(preprocessed_file);
  } catch (const std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << '\n';
    preprocessed_file.close();
    std::remove(preprocessed_file_name.c_str());
    return 1;
  }

  preprocessed_file.close();
  std::remove(preprocessed_file_name.c_str());

  if (flag == "--lex") {
    lexer::PrintTokens(&token_list);
    return 0;
  }

  parser::Program parser_program = parser::ParseProgram(&token_list);

  if (flag == "--parse") {
    std::cout << parser_program.to_string();
    return 0;
  }

  codegen::Program codegen_program = codegen::ParseProgram(parser_program);

  std::string assembly_file = file_prefix + ".s";
  std::ofstream assembly_out(assembly_file.c_str(),
                             std::ofstream::out | std::ofstream::trunc);
  assembly_out << assembler::ParseProgram(codegen_program);
  assembly_out.close();

  std::string assembler_cmd = "gcc " + assembly_file + " -o " + file_prefix;

  int assembler_ret = system(assembler_cmd.c_str());
  std::remove(assembly_file.c_str());

  if (assembler_ret != 0) {
    std::cerr << "Something went wrong assembling the binary " << assembly_file
              << "\n";
  }

  std::cout << "Completed compiling: " << source_file_name << "\n";
  return 0;
}
