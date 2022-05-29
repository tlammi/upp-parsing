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