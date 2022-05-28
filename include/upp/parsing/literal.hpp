#pragma once

#include <optional>

#include "upp/parsing/term.hpp"

namespace upp::parsing {

template <class String, class View>
class Literal final : public TermImpl<View> {
 public:
  Literal(View v) : m_str{v} {}
  Literal(String&& s) : m_str{std::move(s)} {}
  MatchResult<View> match(View v) const noexcept override {
    if (v.size() < m_str.size()) return {false, "", v};
    for (size_t i = 0; i < m_str.size(); ++i) {
      if (v.at(i) != m_str.at(i)) return {false, "", v};
    }
    return {true, v.substr(0, m_str.size()), v.substr(m_str.size())};
  }

 private:
  String m_str;
};
}  // namespace upp::parsing