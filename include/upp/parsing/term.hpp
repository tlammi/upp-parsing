#pragma once

#include <memory>
#include <optional>

#include "upp/parsing/types.hpp"
#include "upp/parsing/util/state.hpp"

namespace upp::parsing {

template <class CharT>
struct MatchResult {
  bool success;
  StringView<CharT> token;
  StringView<CharT> rest;
  constexpr operator bool() const noexcept { return success; }
};

template <class CharT>
class TermImpl {
 public:
  virtual ~TermImpl() {}

  virtual MatchResult<CharT> match(StringView<CharT> view) const noexcept = 0;
};

template <class CharT>
class Term {
 public:
  Term(const util::State<TermImpl<CharT>>& impl) : m_impl{impl} {}
  MatchResult<CharT> match(StringView<CharT> v) const noexcept {
    return m_impl->match(v);
  }

 private:
  util::State<TermImpl<CharT>> m_impl;
};

}  // namespace upp::parsing