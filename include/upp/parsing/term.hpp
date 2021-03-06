#pragma once

#include <limits>
#include <memory>
#include <optional>

#include "upp/parsing/types.hpp"
#include "upp/parsing/util/state.hpp"

namespace upp::parsing {

constexpr size_t NO_MATCH = std::numeric_limits<size_t>::max();

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
  virtual String<CharT> name() const noexcept = 0;
};

template <class CharT, class T, class Cb = std::nullptr_t>
class TermHolder final : public TermImpl<CharT> {
 public:
  TermHolder(T&& t, Cb&& cb = nullptr)
      : m_t{std::move(t)}, m_cb{std::forward<Cb>(cb)} {}
  TermHolder(const T& t, Cb&& cb = nullptr)
      : m_t{t}, m_cb{std::forward<Cb>(cb)} {}

  MatchResult<CharT> match(StringView<CharT> view) const noexcept override {
    size_t res = m_t.match(view);
    if (res == NO_MATCH) {
      return {false, "", view};
    }
    auto match = view.substr(0, res);
    auto rest = view.substr(res);

    if constexpr (!std::is_same_v<Cb, std::nullptr_t>) {
      m_cb(match);
    }

    return {true, match, rest};
  }

  String<CharT> name() const noexcept override { return m_t.name(); }

 private:
  T m_t;
  Cb m_cb{};
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