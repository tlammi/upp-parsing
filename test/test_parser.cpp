#include <gtest/gtest.h>

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