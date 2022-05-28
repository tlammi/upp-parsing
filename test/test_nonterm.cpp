
#include <gtest/gtest.h>

#include "upp/parsing/grammar.hpp"

namespace p = upp::parsing;

TEST(NonTerminal, Ctor) {
  p::Grammar g;
  auto nt = g.nonterminal();
}

TEST(NonTerminal, TerminalExpansion) {
  p::Grammar g;
  auto nt = g.nonterminal();
  auto integer = g.regex(R"(0|[1-9][0-9]*)");
  auto word = g.regex(R"(\S+)");
  nt += (integer, word);
  ASSERT_EQ(nt.expansion_count(), 1);
}