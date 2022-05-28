#pragma once

#include <deque>
#include <string>
#include <string_view>

#include "upp/parsing/literal.hpp"
#include "upp/parsing/nonterm.hpp"
#include "upp/parsing/operators.hpp"
#include "upp/parsing/regex.hpp"
#include "upp/parsing/term.hpp"
#include "upp/parsing/types.hpp"
#include "upp/parsing/util/state.hpp"

namespace upp::parsing {

template <class CharT = char>
class Grammar {
 public:
  Term<CharT> lit(String<CharT> str) {
    using Lit = Literal<CharT>;
    m_terms.emplace_back(new TermHolder<CharT, Lit>{Lit(std::move(str))});
    return {m_terms.back()};
  }

  Term<CharT> regex(String<CharT> str) {
    using Re = Regex<CharT>;
    m_terms.emplace_back(new TermHolder<CharT, Re>{Re(std::move(str))});
    return {m_terms.back()};
  }

  NonTerm<CharT> nonterminal() {
    m_nonterms.emplace_back(new NonTermImpl<CharT>);
    return m_nonterms.back();
  }

 private:
  std::deque<util::State<TermImpl<CharT>>> m_terms{};
  std::deque<util::State<NonTermImpl<CharT>>> m_nonterms{};
};
}  // namespace upp::parsing