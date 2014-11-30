#include "etl/mem/arena.h"

#include <gtest/gtest.h>

namespace etl {
namespace mem {

TEST(ArenaTest_Empty, CreateOkay) {
  Arena<> arena({});
}

TEST(ArenaTest_Empty, ResetOkay) {
  Arena<> arena({});
  arena.reset();
}

TEST(ArenaTest_Empty, AllocateFails) {
  Arena<> arena({});
  arena.reset();

  ASSERT_THROW(arena.allocate(1), std::logic_error);
}

TEST(ArenaTest_Exhaust, ExhaustedBeforeReset) {
  uint8_t mem[64];
  Arena<> arena(mem);

  ASSERT_EQ(64, arena.get_total_count());
  ASSERT_EQ(0, arena.get_free_count());

  ASSERT_THROW(arena.allocate(1), std::logic_error);
}

class ArenaTest : public ::testing::Test {
protected:
  uint8_t region[64];

  Arena<> arena{region};

  virtual void SetUp() {
    arena.reset();
  }

  static constexpr std::size_t expected_total_size = 64;

  bool is_in_region(void * p) {
    return p >= &region[0] && p < &region[expected_total_size];
  }

  void exhaust() {
    arena.allocate(arena.get_free_count());
  }
};

constexpr std::size_t ArenaTest::expected_total_size;

TEST_F(ArenaTest, EmptyAfterReset) {
  ASSERT_EQ(expected_total_size, arena.get_total_count());
  ASSERT_EQ(expected_total_size, arena.get_free_count());
}

TEST_F(ArenaTest, ExhaustWorks) {
  exhaust();
  ASSERT_EQ(0, arena.get_free_count());
  ASSERT_THROW(arena.allocate(1), std::logic_error);
}

TEST_F(ArenaTest, AllocationResultNotNull) {
  ASSERT_NE(nullptr, arena.allocate(10));
}

TEST_F(ArenaTest, AllocationFromRegion) {
  ASSERT_TRUE(is_in_region(arena.allocate(10)));
}

TEST_F(ArenaTest, AllocationUpdatesFreeCount) {
  auto count_before = arena.get_free_count();
  arena.allocate(10);
  ASSERT_TRUE(count_before - arena.get_free_count() >= 10);
}

TEST_F(ArenaTest, AllocationDoesNotAffectTotalCount) {
  auto count_before = arena.get_total_count();
  arena.allocate(10);
  ASSERT_EQ(count_before, arena.get_total_count());
}

TEST_F(ArenaTest, AllocationsDoNotAlias) {
  auto p1 = arena.allocate(10);
  auto p2 = arena.allocate(10);
  ASSERT_NE(p1, p2);
}

TEST_F(ArenaTest, ExcessAllocationAsserts) {
  exhaust();
  ASSERT_THROW(arena.allocate(1), std::logic_error);
}

}  // namespace mem
}  // namespace etl
