#pragma once

#include <memory>
#include <optional>

namespace upp::parsing::grammar {
template <class Iter>
class Matchable {
 public:
  virtual ~Matchable() {}

  virtual std::optional<Iter> match(Iter begin, Iter end) const = 0;

 private:
};
template <class Iter>
class Term {
 public:
  std::optional<Iter> match(Iter begin, Iter end) const {
    return m_match->match(begin, end);
  }

 private:
  std::unique_ptr<Matchable<Iter>> m_match;
};

}  // namespace upp::parsing::grammar