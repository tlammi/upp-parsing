#pragma once

#include "upp/parsing/term.hpp"
#include "upp/parsing/types.hpp"

namespace upp::parsing {

template <class CharT>
class Null {
 public:
  Null() {}

  MatchResult<CharT> match(StringView<CharT> view) const noexcept {
    return {true, "", view};
  }

  String<CharT> name() const { return "<null>"; }

 private:
};

}  // namespace upp::parsing