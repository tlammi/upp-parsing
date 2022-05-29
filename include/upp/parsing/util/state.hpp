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

  T& operator*() noexcept { return **m_ptr; }
  const T& operator*() const noexcept { return **m_ptr; }

  T* operator->() noexcept { return m_ptr->get(); }
  const T* operator->() const noexcept { return m_ptr->get(); }

  template <class... Ts>
  void emplace(Ts&&... ts) {
    *m_ptr = std::make_unique<T>(std::forward<Ts>(ts)...);
  }

  void replace(T* ptr) { *m_ptr = std::unique_ptr<T>{ptr}; }

  size_t ref_count() const noexcept { return m_ptr.use_count(); }

  operator bool() const noexcept { return *m_ptr; }

 private:
  State(std::shared_ptr<std::unique_ptr<T>>&& p) : m_ptr{std::move(p)} {}
  std::shared_ptr<std::unique_ptr<T>> m_ptr;
};

template <class T>
class WeakState {
 public:
  WeakState(const State<T>& state) : m_ptr{state.m_ptr} {}

  WeakState(const WeakState&) = default;
  WeakState& operator=(const WeakState&) = default;

  WeakState(WeakState&&) noexcept = default;
  WeakState& operator=(WeakState&&) noexcept = default;

  State<T> lock() { return {m_ptr.lock()}; }

 private:
  std::weak_ptr<std::unique_ptr<T>> m_ptr;
};
}  // namespace upp::parsing::util
