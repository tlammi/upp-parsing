#pragma once

#include <deque>
#include <string>
#include <string_view>

#include "upp/parsing/end.hpp"
#include "upp/parsing/literal.hpp"
#include "upp/parsing/nonterm.hpp"
#include "upp/parsing/null.hpp"
#include "upp/parsing/operators.hpp"
#include "upp/parsing/regex.hpp"
#include "upp/parsing/term.hpp"
#include "upp/parsing/types.hpp"
#include "upp/parsing/util/state.hpp"

namespace upp::parsing {

template <class CharT>
class Parser;

template <class CharT = char>
class Grammar {
  friend class Parser<CharT>;

 public:
  template <class Cb = std::nullptr_t>
  Term<CharT> lit(String<CharT> str, Cb&& cb = nullptr) {
    using Lit = Literal<CharT>;
    m_terms.emplace_back(new TermHolder<CharT, Lit, Cb>{Lit(std::move(str)),
                                                        std::forward<Cb>(cb)});
    return {m_terms.back()};
  }

  template <class Cb = std::nullptr_t>
  Term<CharT> regex(String<CharT> str, Cb&& cb = nullptr) {
    using Re = Regex<CharT>;
    m_terms.emplace_back(new TermHolder<CharT, Re, Cb>{Re(std::move(str)),
                                                       std::forward<Cb>(cb)});
    return {m_terms.back()};
  }

  Term<CharT> null() {
    using N = Null<CharT>;
    m_terms.emplace_back(new TermHolder<CharT, N>{N{}});
    return {m_terms.back()};
  }

  Term<CharT> end() {
    using E = End<CharT>;
    m_terms.emplace_back(new TermHolder<CharT, E>{E{}});
    return m_terms.back();
  }

  NonTerm<CharT> nonterminal() {
    m_nonterms.emplace_back(new NonTermImpl<CharT>);
    return m_nonterms.back();
  }
  NonTerm<CharT> nonterminal(StringView<CharT> name) {
    m_nonterms.emplace_back(new NonTermImpl<CharT>{name});
    return m_nonterms.back();
  }

  String<CharT> grammar_string() const {
    std::stringstream ss;
    for (const auto& nt : m_nonterms) {
      for (const auto& exp : nt->expansions()) {
        ss << nt->name() << "->";
        for (const auto& e : exp) {
          if (is_terminal(e)) {
            ss << " " << to_terminal(e).lock()->name();
          } else {
            ss << " " << to_non_terminal(e).lock()->name();
          }
        }
        ss << '\n';
      }
    }
    return ss.str();
  }

 private:
  std::deque<util::State<TermImpl<CharT>>> m_terms{};
  std::deque<util::State<NonTermImpl<CharT>>> m_nonterms{};
};
}  // namespace upp::parsing