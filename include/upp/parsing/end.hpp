#pragma once

#include "upp/parsing/term.hpp"
#include "upp/parsing/types.hpp"
namespace upp::parsing {

template <class CharT>
class End {
 public:
  End() {}

  size_t match(StringView<CharT> view) const noexcept {
    if (view.empty()) return 0;
    return NO_MATCH;
  }

  String<CharT> name() const { return "<end>"; }

 private:
};
}  // namespace upp::parsing