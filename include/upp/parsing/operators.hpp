#pragma once

#include "upp/parsing/nonterm.hpp"
#include "upp/parsing/term.hpp"

namespace upp::parsing {

template <class CharT>
Expansion<CharT> operator,(const Term<CharT>& l, const Term<CharT>& r) {
  return {l.impl(), r.impl()};
}

template <class CharT>
Expansion<CharT> operator,(Expansion<CharT>&& l, const Term<CharT>& r) {
  l.emplace_back(r.impl());
  return l;
}
template <class CharT>
Expansion<CharT> operator,(const Term<CharT>& l, Expansion<CharT>&& r) {
  r.emplace_back(l.impl());
  return r;
}

template <class CharT>
Expansion<CharT> operator,(const NonTerm<CharT>& l, const NonTerm<CharT>& r) {
  return {l.impl(), r.impl()};
}

template <class CharT>
Expansion<CharT> operator,(Expansion<CharT>&& l, const NonTerm<CharT>& r) {
  l.emplace_back(r.impl());
  return l;
}

template <class CharT>
Expansion<CharT> operator,(const NonTerm<CharT>& l, Expansion<CharT>&& r) {
  r.emplace_back(l.impl());
  return r;
}

template <class CharT>
Expansion<CharT> operator,(const Term<CharT>& l, const NonTerm<CharT>& r) {
  return {l.impl(), r.impl()};
}

template <class CharT>
Expansion<CharT> operator,(const NonTerm<CharT>& l, const Term<CharT>& r) {
  return {l.impl(), r.impl()};
}
}  // namespace upp::parsing