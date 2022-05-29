#pragma once

#include <iostream>

#include "upp/parsing/grammar.hpp"
namespace upp::parsing {

namespace detail {

template <class CharT>
String<CharT> expansion_debug(const Expansion<CharT>& expansion) {
  if (expansion.empty()) return "[]";
  String<CharT> out{"[ "};
  for (const auto& e : expansion) {
    if (is_terminal(e)) {
      out += to_terminal(e).lock()->name() + " ";
    } else {
      out += String<CharT>{to_non_terminal(e).lock()->name()} + " ";
    }
  }
  out += "]";
  return out;
}

/**
 * LL(1) check for checking if expansion matches the view
 *
 * LL(1) grammars mean that non-terminal expansion can be decided by using one
 * token lookahead.
 */
template <class CharT>
bool ll1_check(const Ast<CharT>& a, StringView<CharT> view) {
  if (is_terminal(a)) {
    util::State<TermImpl<CharT>> term = to_terminal(a).lock();
    return term->match(view);
  } else {
    util::State<NonTermImpl<CharT>> non_term = to_non_terminal(a).lock();
    for (const auto& exp : non_term->expansions()) {
      std::cerr << "debug: " << expansion_debug(exp) << '\n';
      if (ll1_check(exp.front(), view)) return true;
    }
  }
  return false;
}

template <class CharT>
bool match_expansion(const Expansion<CharT>& expansion,
                     StringView<CharT> view) {
  return ll1_check(expansion.front(), view);
}

}  // namespace detail

template <class CharT>
class Parser {
 public:
  Parser(Grammar<CharT>&& g, const NonTerm<CharT>& starting_nonterm)
      : m_terms{std::move(g.m_terms)},
        m_nonterms{std::move(g.m_nonterms)},
        m_starting_nonterm{starting_nonterm.impl()} {}

  bool parse(StringView<CharT> view) const noexcept {
    Expansion<CharT> expansion{util::WeakState{m_starting_nonterm}};
    while (!expansion.empty()) {
      while (!view.empty() && std::isspace(view.front())) view.remove_prefix(1);
      std::cerr << "matching \"" << view << "\"\n";
      std::cerr << detail::expansion_debug(expansion) << '\n';
      if (is_terminal(expansion.front())) {
        auto term = to_terminal(expansion.front()).lock();
        auto [success, token, rest] = term->match(view);
        if (success) {
          std::cerr << "match: " << token << '\n';
          std::cerr << "left with: \"" << rest << "\"\n";
          view = rest;
          expansion.pop_front();
          std::cerr << "work queue size: " << expansion.size() << '\n';
        } else {
          std::cerr << "mismatch\n";
          return false;
        }
      } else {
        auto nonterm = to_non_terminal(expansion.front()).lock();
        bool match_found = false;
        for (const auto& exp : nonterm->expansions()) {
          if (detail::match_expansion(exp, view)) {
            expansion.pop_front();
            expansion.insert(expansion.begin(), exp.begin(), exp.end());
            match_found = true;
            std::cerr << "non-terminal expansion found\n";
            break;
          }
        }
        if (!match_found) return false;
      }
    }
    return true;
  }

 private:
  std::deque<util::State<TermImpl<CharT>>> m_terms{};
  std::deque<util::State<NonTermImpl<CharT>>> m_nonterms{};
  util::State<NonTermImpl<CharT>> m_starting_nonterm{};
};

}  // namespace upp::parsing