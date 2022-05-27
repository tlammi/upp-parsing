#pragma once

#include <cstddef>
#include <utility>

namespace upp::parsing::util {
namespace detail {
template <class T>
struct Ctx {
  Ctx() {}
  Ctx(const Ctx&) = delete;
  Ctx& operator=(const Ctx&) = delete;

  Ctx(Ctx&&) = delete;
  Ctx& operator=(Ctx&&) = delete;

  ~Ctx() { delete ptr; }

  T* ptr{nullptr};
  size_t ref_count{0};
};
}  // namespace detail

template <class T>
class WeakState;

template <class T>
class State {
  using Ctx = detail::Ctx<T>;
  friend class WeakState<T>;

 public:
  State() : State(new Ctx) {}
  State(T* ptr) : State(new Ctx) { m_ctx->ptr = ptr; }

  State(const State& other) : m_ctx{other.m_ctx} { ++m_ctx->ref_count; }
  State& operator=(const State& other) {
    --m_ctx->ref_count;
    if (!m_ctx->ref_count) delete m_ctx;
    m_ctx = other.m_ctx;
    ++m_ctx->ref_count;
    return *this;
  }

  State(State&&) = delete;
  State& operator=(State&&) = delete;

  ~State() {
    --m_ctx->ref_count;
    if (!m_ctx->ref_count) delete m_ctx;
  }

  T& operator*() noexcept { return *m_ctx->ptr; }
  const T& operator*() const noexcept { return *m_ctx->ptr; }

  T* operator->() noexcept { return m_ctx->ptr; }
  const T* operator->() const noexcept { return m_ctx->ptr; }

  template <class... Ts>
  void emplace(Ts&&... ts) {
    delete m_ctx->ptr;
    m_ctx->ptr = new T{std::forward<Ts>(ts)...};
  }

  void replace(T* ptr) {
    delete m_ctx->ptr;
    m_ctx->ptr = ptr;
  }

  size_t ref_count() const noexcept { return m_ctx->ref_count; }

 private:
  State(Ctx* ctx) : m_ctx{ctx} { ++m_ctx->ref_count; }
  Ctx* m_ctx;
};

template <class T>
class WeakState {
  using Ctx = detail::Ctx<T>;

 public:
  WeakState(const State<T>& state) : m_ctx{state.m_ctx} {}

  State<T> lock() { return {m_ctx}; }

 private:
  Ctx* m_ctx;
};
}  // namespace upp::parsing::util
