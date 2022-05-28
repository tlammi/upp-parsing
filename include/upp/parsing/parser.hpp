#pragma once

#include "upp/parsing/grammar.hpp"
namespace upp::parsing {

template <class CharT>
class Parser {
 public:
  Parser(Grammar<CharT>&& g, const NonTerm<CharT>& starting_nonterm)
      : m_terms{std::move(g.m_terms)},
        m_nonterms{std::move(g.m_nonterms)},
        m_starting_nonterm{starting_nonterm.impl()} {}

 private:
  std::deque<util::State<TermImpl<CharT>>> m_terms{};
  std::deque<util::State<NonTermImpl<CharT>>> m_nonterms{};
  util::State<NonTermImpl<CharT>> m_starting_nonterm{};
};

}  // namespace upp::parsing