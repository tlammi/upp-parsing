#include <gtest/gtest.h>

#include "upp/parsing/util/state.hpp"

namespace u = upp::parsing::util;

TEST(Ctor, Simple) {
  u::State<int> s0{};
  u::State<double> s1{};
  u::State<int> s2{new int};
}

TEST(Ctor, Share) {
  u::State<int> s0{};
  u::State<int> s1{s0};
  u::State<int> s2{s1};
  u::WeakState w{s0};
}

TEST(RefCount, State) {
  u::State<int> s0{};
  ASSERT_EQ(s0.ref_count(), 1);
  {
    u::State s1{s0};
    ASSERT_EQ(s0.ref_count(), 2);
    ASSERT_EQ(s1.ref_count(), 2);
    {
      u::State s2{s0};
      ASSERT_EQ(s0.ref_count(), 3);
      ASSERT_EQ(s1.ref_count(), 3);
      ASSERT_EQ(s2.ref_count(), 3);
      {
        u::State s3{s2};
        ASSERT_EQ(s0.ref_count(), 4);
        ASSERT_EQ(s1.ref_count(), 4);
        ASSERT_EQ(s2.ref_count(), 4);
        ASSERT_EQ(s3.ref_count(), 4);
      }
      ASSERT_EQ(s0.ref_count(), 3);
      ASSERT_EQ(s1.ref_count(), 3);
      ASSERT_EQ(s2.ref_count(), 3);
    }
    ASSERT_EQ(s0.ref_count(), 2);
    ASSERT_EQ(s1.ref_count(), 2);
  }
  ASSERT_EQ(s0.ref_count(), 1);
}

TEST(RefCount, Weak) {
  u::State<int> s{new int};
  u::WeakState w{s};
  ASSERT_EQ(s.ref_count(), 1);
  ASSERT_EQ(w.lock().ref_count(), 2);
  ASSERT_EQ(s.ref_count(), 1);
}

struct AbstractDefer {
  virtual ~AbstractDefer() {}
};
template <class C>
struct Defer : public AbstractDefer {
  Defer(C&& c) : c{std::forward<C>(c)} {}
  ~Defer() { c(); }

  C c;
};

TEST(Dtor, One) {
  int counter = 0;
  {
    u::State s{new Defer{[&]() { ++counter; }}};
  }
  ASSERT_EQ(counter, 1);
}

TEST(Dtor, Multiple) {
  int counter = 0;
  {
    u::State s0{new Defer{[&]() { ++counter; }}};
    u::State s1{s0};
    u::State s2{s0};
    u::State s3{s0};
  }
  ASSERT_EQ(counter, 1);
}

TEST(Dtor, Replace) {
  int counter = 0;
  {
    u::State<AbstractDefer> s{new Defer{[&]() { ++counter; }}};
    s.replace(new Defer{[&]() { counter += 2; }});
    ASSERT_EQ(counter, 1);
  }
  ASSERT_EQ(counter, 3);
}

TEST(Emplace, Simple) {
  struct A {
    A(int a, int b) : a{a}, b{b} {}
    int a;
    int b;
  };

  {
    u::State<A> s{new A{1, 2}};
    ASSERT_EQ(s->a, 1);
    ASSERT_EQ(s->b, 2);

    s.emplace(100, 200);
    ASSERT_EQ(s->a, 100);
    ASSERT_EQ(s->b, 200);
  }
}