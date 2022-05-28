#pragma once

#include <deque>
#include <string>
#include <string_view>

#include "upp/parsing/literal.hpp"
#include "upp/parsing/term.hpp"
#include "upp/parsing/types.hpp"
#include "upp/parsing/util/state.hpp"

namespace upp::parsing {

template <class CharT = char>
class Grammar {
 public:
  Term<CharT> lit(String<CharT>&& str) {
    m_terms.emplace_back(new Literal<CharT>(std::move(str)));
    return {m_terms.back()};
  }

 private:
  std::deque<util::State<TermImpl<CharT>>> m_terms{};
};
}  // namespace upp::parsing