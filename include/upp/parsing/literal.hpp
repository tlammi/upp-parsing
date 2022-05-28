#pragma once

#include <optional>

#include "upp/parsing/term.hpp"
#include "upp/parsing/types.hpp"

namespace upp::parsing {

template <class CharT>
class Literal final : public TermImpl<CharT> {
 public:
  Literal(StringView<CharT> v) : m_str{v} {}
  Literal(String<CharT>&& s) : m_str{std::move(s)} {}
  MatchResult<CharT> match(StringView<CharT> v) const noexcept override {
    if (v.size() < m_str.size()) return {false, "", v};
    for (size_t i = 0; i < m_str.size(); ++i) {
      if (v.at(i) != m_str.at(i)) return {false, "", v};
    }
    return {true, v.substr(0, m_str.size()), v.substr(m_str.size())};
  }

 private:
  String<CharT> m_str;
};
}  // namespace upp::parsing