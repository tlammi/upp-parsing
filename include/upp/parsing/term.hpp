#pragma once

#include <memory>
#include <optional>

#include "upp/parsing/util/state.hpp"

namespace upp::parsing {

template <class View>
struct MatchResult {
  bool success;
  View token;
  View rest;
  constexpr operator bool() const noexcept { return success; }
};

template <class View>
class TermImpl {
 public:
  virtual ~TermImpl() {}

  virtual MatchResult<View> match(View view) const noexcept = 0;
};

template <class View>
class Term {
 public:
  Term(const util::State<TermImpl<View>>& impl) : m_impl{impl} {}
  MatchResult<View> match(View v) const noexcept { return m_impl->match(v); }

 private:
  util::State<TermImpl<View>> m_impl;
};

}  // namespace upp::parsing