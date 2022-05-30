#pragma once

#include "upp/parsing/term.hpp"
#include "upp/parsing/types.hpp"

namespace upp::parsing {

template <class CharT>
class Null {
 public:
  Null() {}

  size_t match(StringView<CharT> view) const noexcept { return 0; }

  String<CharT> name() const { return "<null>"; }

 private:
};

}  // namespace upp::parsing