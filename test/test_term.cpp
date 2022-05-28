#include <gtest/gtest.h>

#include "upp/parsing/grammar.hpp"

namespace p = upp::parsing;

void test_extact(const p::Term<std::string_view>& t,
                 std::string_view to_match) {
  auto match = t.match(to_match);
  ASSERT_TRUE(match);
  ASSERT_EQ(match.token, to_match);
  ASSERT_EQ(match.rest, "");
}

void test_match(const p::Term<std::string_view>& t, std::string_view str) {
  auto match = t.match(str);
  ASSERT_TRUE(match);
  auto substr0 = str.substr(0, match.token.size());
  auto substr1 = str.substr(match.token.size());
  ASSERT_EQ(match.token, substr0);
  ASSERT_EQ(match.rest, substr1);
}

void test_no_match(const p::Term<std::string_view>& t, std::string_view str) {
  auto match = t.match(str);
  ASSERT_FALSE(match);
  ASSERT_EQ(match.token, "");
  ASSERT_EQ(match.rest, str);
}

TEST(Literal, Exact) {
  p::Grammar g;
  auto t = g.lit("foo");
  test_extact(t, "foo");
}

TEST(Literal, Match) {
  p::Grammar g;
  auto t = g.lit("foo");
  test_match(t, "foobar");
}

TEST(Literal, TooShort) {
  p::Grammar g;
  auto t = g.lit("foo");
  test_no_match(t, "fo");
}

TEST(Literal, NoMatch) {
  p::Grammar g;
  auto t = g.lit("foo");
  test_no_match(t, "asdfasdfasdf");
}
