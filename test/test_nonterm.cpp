
#include <gtest/gtest.h>

#include "upp/parsing/grammar.hpp"

namespace p = upp::parsing;

TEST(NonTerminal, Ctor) {
  p::Grammar g;
  auto nt = g.nonterminal();
}