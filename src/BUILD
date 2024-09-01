cc_binary(
  name = "compiler",
  srcs = ["main.cc"],
  deps = [
    "//src/internal/assembler:assembler",
    "//src/internal/codegen:codegen",
    "//src/internal/lexer:lexer",
    "//src/internal/parser:parser",
  ],
  copts = ["-std=c++17"],
)