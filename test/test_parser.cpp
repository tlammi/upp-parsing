#include <gtest/gtest.h>

#include "upp/parsing/parser.hpp"

namespace p = upp::parsing;

TEST(Parser, Ctor) {
  p::Grammar g{};
  auto nt = g.nonterminal();
  p::Parser p{std::move(g), nt};
}