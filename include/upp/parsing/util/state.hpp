#pragma once

#include <cstddef>
#include <utility>

namespace upp::parsing::util {

template <class T>
class WeakState;

/**
 * Pointer type where data can be replaced and all owners see the change
 *
 * Implemented pretty much as a std::shared_ptr<std::unique_ptr<T>>
 *
 */
template <class T>
class State {
  friend class WeakState<T>;

 public:
  State() : m_ptr{new std::unique_ptr<T>()} {}
  State(T* ptr) : m_ptr{new std::unique_ptr<T>(ptr)} {}

  State(const State&) = default;
  State& operator=(const State&) = default;

  State(State&&) noexcept = default;
  State& operator=(State&&) noexcept = default;

  ~State() {}

  T& operator*() noexcept { return *m_ptr->ptr; }
  const T& operator*() const noexcept { return *m_ptr->ptr; }

  T* operator->() noexcept { return m_ptr->ptr; }
  const T* operator->() const noexcept { return m_ptr->ptr; }

  template <class... Ts>
  void emplace(Ts&&... ts) {
    m_ptr->ptr = std::make_unique<T>(std::forward<Ts>(ts)...);
  }

  void replace(T* ptr) { m_ptr->ptr = std::unique_ptr(ptr); }

  size_t ref_count() const noexcept { return m_ptr.use_count(); }

 private:
  State(std::shared_ptr<std::unique_ptr<T>>&& p) : m_ptr{std::move(p)} {}
  std::shared_ptr<std::unique_ptr<T>> m_ptr;
};

template <class T>
class WeakState {
 public:
  WeakState(const State<T>& state) : m_ptr{state.m_ptr} {}

  State<T> lock() { return {m_ptr.lock()}; }

 private:
  std::weak_ptr<std::unique_ptr<T>> m_ptr;
};
}  // namespace upp::parsing::util
