#pragma once

#include <deque>
#include <string>
#include <string_view>

#include "upp/parsing/literal.hpp"
#include "upp/parsing/term.hpp"
#include "upp/parsing/util/state.hpp"

namespace upp::parsing {

template <class String = std::string, class View = std::string_view>
class Grammar {
 public:
  using string_type = String;
  using view_type = View;

  Term<View> lit(String&& str) {
    m_terms.emplace_back(new Literal<String, View>(std::move(str)));
    return {m_terms.back()};
  }

 private:
  std::deque<util::State<TermImpl<View>>> m_terms{};
};
}  // namespace upp::parsing