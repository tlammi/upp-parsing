#pragma once

#include <regex>

#include "upp/parsing/term.hpp"
#include "upp/parsing/types.hpp"

namespace upp::parsing {

template <class CharT>
class Regex {
 public:
  Regex(String<CharT> s) : m_str{"re'" + s + "'"}, m_re{std::move(s)} {}

  MatchResult<CharT> match(StringView<CharT> v) const noexcept {
    std::match_results<typename StringView<CharT>::const_iterator> match_result;
    bool found = std::regex_search(v.cbegin(), v.cend(), match_result, m_re,
                                   std::regex_constants::match_continuous |
                                       std::regex_constants::match_not_null);
    if (!found) return {false, "", v};
    auto match_substr = v.substr(0, match_result.length());
    auto rest_substr = v.substr(match_result.length());
    return {true, match_substr, rest_substr};
  }

  String<CharT> name() const noexcept { return m_str; }

 private:
  String<CharT> m_str{};
  std::basic_regex<CharT> m_re{};
};
}  // namespace upp::parsing