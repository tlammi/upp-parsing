#include <gtest/gtest.h>

#include <fstream>

#include "upp/parsing/parser.hpp"

namespace p = upp::parsing;

TEST(Ctor, Empty) {
  p::Grammar g{};
  auto nt = g.nonterminal();
  p::Parser p{std::move(g), nt};
}
TEST(Parse, TerminalOnly) {
  p::Grammar g{};
  auto nt = g.nonterminal();
  nt += (g.lit("foo"), g.lit("bar"));
  p::Parser p{std::move(g), nt};
  std::string_view input_success{"foo bar"};
  ASSERT_TRUE(p.parse(input_success));
  std::string_view input_failure{"baz"};
  ASSERT_FALSE(p.parse(input_failure));
}

TEST(Parse, Alternatives) {
  p::Grammar g;
  auto boolean = g.nonterminal();
  boolean += g.lit("true");
  boolean += g.lit("false");
  p::Parser p{std::move(g), boolean};
  ASSERT_TRUE(p.parse("true"));
  ASSERT_TRUE(p.parse("false"));
  ASSERT_FALSE(p.parse("tru"));
}

TEST(Parse, Callback) {
  p::Grammar g;
  auto two_booleans = g.nonterminal();
  bool true_called = false;
  bool false_called = false;
  two_booleans += (g.lit("true",
                         [&](std::string_view str) {
                           EXPECT_EQ(str, "true");
                           true_called = true;
                         }),
                   g.lit("false", [&](std::string_view str) {
                     EXPECT_EQ(str, "false");
                     false_called = true;
                   }));

  p::Parser p{std::move(g), two_booleans};
  p.parse("true  false");
  ASSERT_TRUE(true_called);
  ASSERT_TRUE(false_called);
}

TEST(Parse, Recursion) {
  p::Grammar g;
  auto boolean = g.nonterminal();
  size_t true_count = 0;
  size_t false_count = 0;
  boolean += g.lit("true", [&](std::string_view) { ++true_count; });
  boolean += g.lit("false", [&](std::string_view) { ++false_count; });
  auto recursive = g.nonterminal();
  recursive += (boolean, recursive);
  recursive += g.end();
  p::Parser p{std::move(g), recursive};
  std::string_view input = "true false true false false false true";
  ASSERT_TRUE(p.parse(input));
}

TEST(Parse, Json) {
  p::Grammar g;
  auto value = g.nonterminal("value");
  auto null = g.lit("null");
  auto boolean = g.nonterminal("bool");
  boolean += g.lit("true");
  boolean += g.lit("false");
  auto integer = g.regex("0|[0-9][1-9]*");
  auto fpoint = g.regex(R"((0|[1-9][0-9]*)\.[0-9]+)");
  auto string = g.regex(R"(".*?[^\\]")");
  auto key_value = g.nonterminal("key_value");
  auto obj_rest = g.nonterminal("obj_rest");
  auto obj_rest2 = g.nonterminal("obj_rest2");
  auto obj = g.nonterminal("obj");
  auto arr = g.nonterminal("arr");
  auto arr_rest = g.nonterminal("arr_rest");
  auto arr_rest2 = g.nonterminal("arr_rest2");

  value += null;
  value += boolean;
  value += fpoint;
  value += integer;
  value += string;
  value += obj;
  value += arr;

  key_value += (string, g.lit(":"), value);
  obj += (g.lit("{"), obj_rest);
  obj_rest += g.lit("}");
  obj_rest += (key_value, obj_rest2);
  obj_rest2 += g.lit("}");
  obj_rest2 += (g.lit(","), key_value, obj_rest2);

  arr += (g.lit("["), arr_rest);
  arr_rest += g.lit("]");
  arr_rest += (value, arr_rest2);
  arr_rest2 += g.lit("]");
  arr_rest2 += (g.lit(","), value, arr_rest2);

  p::Parser p{std::move(g), value};

  std::string_view json = R"(
      {
        "hello": {
            "text": "world"
        },
        "arr": ["this", "is", "a", "list", 0.0, true, false]
      }
  )";

  std::ifstream fs{"large.json"};
  std::istream_iterator<char> begin{fs};
  std::istream_iterator<char> end{};
  std::string large{begin, end};

  ASSERT_TRUE(p.parse(large));
  // ASSERT_TRUE(p.parse(json));
}