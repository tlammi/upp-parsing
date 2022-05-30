#pragma once

#include <optional>

#include "upp/parsing/term.hpp"
#include "upp/parsing/types.hpp"

namespace upp::parsing {

template <class CharT>
class Literal {
 public:
  Literal(StringView<CharT> v) : m_str{v} {}
  Literal(String<CharT>&& s) : m_str{std::move(s)} {}
  size_t match(StringView<CharT> v) const noexcept {
    if (v.size() < m_str.size()) return NO_MATCH;
    for (size_t i = 0; i < m_str.size(); ++i) {
      if (v.at(i) != m_str.at(i)) return NO_MATCH;
    }
    return m_str.size();
  }

  String<CharT> name() const noexcept { return "'" + m_str + "'"; }

 private:
  String<CharT> m_str;
};
}  // namespace upp::parsing