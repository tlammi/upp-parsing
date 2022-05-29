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
bool is_terminal(const Ast<CharT>& a) {
  return std::holds_alternative<util::WeakState<TermImpl<CharT>>>(a);
}

template <class CharT>
util::WeakState<TermImpl<CharT>> to_terminal(const Ast<CharT>& a) {
  return std::get<util::WeakState<TermImpl<CharT>>>(a);
}

template <class CharT>
util::WeakState<NonTermImpl<CharT>> to_non_terminal(const Ast<CharT>& a) {
  return std::get<util::WeakState<NonTermImpl<CharT>>>(a);
}

template <class CharT>
using Expansion = std::deque<Ast<CharT>>;

template <class CharT>
using ExpansionList = std::deque<Expansion<CharT>>;

template <class CharT>
class NonTermImpl {
 public:
  NonTermImpl() {}

  NonTermImpl& operator+=(Expansion<CharT>&& e) {
    m_expansions.push_back(std::move(e));
    return *this;
  }
  NonTermImpl& operator+=(const Term<CharT>& t) {
    m_expansions.emplace_back();
    m_expansions.back().push_back(t.impl());
    return *this;
  }

  size_t expansion_count() const noexcept { return m_expansions.size(); }

  const ExpansionList<CharT>& expansions() const noexcept {
    return m_expansions;
  }

 private:
  ExpansionList<CharT> m_expansions{};
};

template <class CharT>
class NonTerm {
 public:
  NonTerm(const util::State<NonTermImpl<CharT>>& impl) : m_impl{impl} {}

  NonTerm& operator+=(Expansion<CharT>&& e) {
    *m_impl += std::move(e);
    return *this;
  }

  NonTerm& operator+=(const Term<CharT>& t) {
    *m_impl += t;
    return *this;
  }

  size_t expansion_count() const noexcept { return m_impl->expansion_count(); }

  const util::State<NonTermImpl<CharT>>& impl() const noexcept {
    return m_impl;
  }

 private:
  util::State<NonTermImpl<CharT>> m_impl;
};
}  // namespace upp::parsing