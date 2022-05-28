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

template <class CharT, class T>
class TermHolder final : public TermImpl<CharT> {
 public:
  TermHolder(T&& t) : m_t{std::move(t)} {}
  TermHolder(const T& t) : m_t{t} {}

  MatchResult<CharT> match(StringView<CharT> view) const noexcept override {
    return m_t.match(view);
  }

 private:
  T m_t;
};

template <class CharT>
class Term {
 public:
  Term(const util::State<TermImpl<CharT>>& impl) : m_impl{impl} {}
  MatchResult<CharT> match(StringView<CharT> v) const noexcept {
    return m_impl->match(v);
  }

  const util::State<TermImpl<CharT>>& impl() const noexcept { return m_impl; }

 private:
  util::State<TermImpl<CharT>> m_impl;
};

}  // namespace upp::parsing