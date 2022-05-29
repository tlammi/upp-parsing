#pragma once

#include "upp/parsing/term.hpp"
#include "upp/parsing/types.hpp"
namespace upp::parsing {

template <class CharT>
class End {
 public:
  End() {}

  MatchResult<CharT> match(StringView<CharT> view) const noexcept {
    if (view.empty()) return {true, "", view};
    return {false, "", view};
  }

  String<CharT> name() const { return "<end>"; }

 private:
};
}  // namespace upp::parsing