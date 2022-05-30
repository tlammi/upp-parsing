#pragma once

#include <regex>

#include "upp/parsing/term.hpp"
#include "upp/parsing/types.hpp"

namespace upp::parsing {

template <class CharT>
class Regex {
 public:
  Regex(String<CharT> s) : m_str{"re'" + s + "'"}, m_re{std::move(s)} {}

  size_t match(StringView<CharT> v) const noexcept {
    std::match_results<typename StringView<CharT>::const_iterator> match_result;
    bool found = std::regex_search(v.cbegin(), v.cend(), match_result, m_re,
                                   std::regex_constants::match_continuous |
                                       std::regex_constants::match_not_null);
    if (!found) return NO_MATCH;
    return match_result.length();
  }

  String<CharT> name() const noexcept { return m_str; }

 private:
  String<CharT> m_str{};
  std::basic_regex<CharT> m_re{};
};
}  // namespace upp::parsing