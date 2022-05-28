#pragma once

#include <string>
#include <string_view>

namespace upp::parsing {

template <class CharT = char>
using String = std::basic_string<CharT>;

template <class CharT = char>
using StringView = std::basic_string_view<CharT>;

}  // namespace upp::parsing