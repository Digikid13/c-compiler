Just a crappy WIP of a C compiler written in C++. My goal is to learn C++ while completing the book "Writing a C Compiler: Build a Real Programming Language from Scratch" so there's going to be a lot of bad C++ code in here.

Requirements:  

This project uses [bazel](https://bazel.build/) to manage dependencies and build the project.  
You can install it here: https://bazel.build/start

[blzmod](https://bazel.build/external/overview#bzlmod) is enabled to pull deps from the [Bazel Central Registry](https://registry.bazel.build/).

Build:  
`bazel build //src:compiler`

Test:  
`bazel test <path_to_test>`  
(e.g. `bazel test //src/internal/lexer:lexer_test`)

Run:  
`./bazel-bin/src/compiler [--lex|--parse|--codegen] <file_name>`
