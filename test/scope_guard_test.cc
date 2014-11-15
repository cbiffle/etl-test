#include "etl/scope_guard.h"

#include <gtest/gtest.h>

TEST(ScopeGuard, Basic) {
  unsigned count = 0;

  {
    auto guard = etl::make_guard([&] { ++count; });
    ASSERT_EQ(0, count);
  }

  ASSERT_EQ(1, count);
}

TEST(ScopeGuard, Multiple) {
  unsigned count = 0;

  {
    auto guard1 = etl::make_guard([&] { ++count; });
    auto guard2 = etl::make_guard([&] { ++count; });
    auto guard3 = etl::make_guard([&] { ++count; });
    ASSERT_EQ(0, count);
  }

  ASSERT_EQ(3, count);
}

TEST(ScopeGuard, Dismissed) {
  unsigned count = 0;

  {
    auto guard = etl::make_guard([&] { ++count; });
    ASSERT_EQ(0, count);
    guard.dismiss();
  }

  ASSERT_EQ(0, count);
}

TEST(ScopeGuard, DismissedImplicitlyOnMove) {
  unsigned count = 0;

  {
    auto guard = etl::make_guard([&] { ++count; });
    ASSERT_EQ(0, count);

    // This is a little contrived -- normally it would be moved on return
    // from a function or some such.
    auto guard2 = etl::move(guard);
    ASSERT_EQ(0, count);
  }

  ASSERT_EQ(1, count);
}

TEST(ScopeGuard, OnScopeExitMacro) {
  unsigned count = 0;

  {
    ETL_ON_SCOPE_EXIT { ++count; };
    ASSERT_EQ(0, count);
  }

  ASSERT_EQ(1, count);
}


