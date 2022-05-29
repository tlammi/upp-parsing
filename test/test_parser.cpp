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
  std::string_view input{"foo bar"};
  ASSERT_TRUE(p.parse(input));
}