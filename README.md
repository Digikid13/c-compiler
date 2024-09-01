Just a crappy WIP of a C compiler written in C++. My goal is to learn C++ while completing the book "Writing a C Compiler: Build a Real Programming Language from Scratch" so there's going to be a lot of bad C++ code in here.

Build:
`bazel build //src:compiler`
git 
Run:
`bazel-bin/src/compiler [--lex|--parse|--codegen] <file_name>`
