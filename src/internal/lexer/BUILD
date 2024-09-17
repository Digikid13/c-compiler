cc_test(
  name = "lexer_test",
  size = "small",
  srcs = ["lexer_test.cc"],
  deps = [
    ":lexer",
    "@googletest//:gtest",
    "@googletest//:gtest_main"
  ],
)

cc_library(
  name = "lexer",
  srcs = ["lexer.cc"],
  hdrs = ["lexer.h"],
  deps = ["@re2//:re2"],
  visibility = ["//visibility:public"],
)
