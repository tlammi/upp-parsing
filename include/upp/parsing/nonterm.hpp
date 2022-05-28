#pragma once

#include <deque>
#include <variant>

#include "upp/parsing/term.hpp"
#include "upp/parsing/util/state.hpp"

namespace upp::parsing {

template <class CharT>
class NonTermImpl;

template <class... Ts>
using StateVariant = std::variant<util::WeakState<Ts>...>;

template <class CharT>
using Ast = StateVariant<TermImpl<CharT>, NonTermImpl<CharT>>;

template <class CharT>
const TermImpl<CharT>* to_terminal(const Ast<CharT>* a) {
  return std::get<TermImpl<CharT>>(a);
}

template <class CharT>
const TermImpl<CharT>* to_non_terminal(const Ast<CharT>* a) {
  return std::get<NonTermImpl<CharT>>(a);
}

template <class CharT>
using Expansion = std::deque<Ast<CharT>>;

template <class CharT>
class NonTermImpl {
 public:
  NonTermImpl() {}

 private:
  std::deque<Expansion<CharT>> m_expansions{};
};

template <class CharT>
class NonTerm {
 public:
  NonTerm(const util::State<NonTermImpl<CharT>>& impl) : m_impl{impl} {}

 private:
  util::State<NonTermImpl<CharT>> m_impl;
};
}  // namespace upp::parsing