#include "etl/mem/arena.h"

#include <gtest/gtest.h>

namespace etl {
namespace mem {

TEST(ArenaTest, CreateEmptyOkay) {
  Arena arena({});
}

TEST(ArenaTest, ResetEmptyOkay) {
  Arena arena({});
  arena.reset();
}

TEST(ArenaTest, AllocateEmptyFails) {
  Arena arena({});
  arena.reset();

  ASSERT_THROW(arena.allocate(1), std::logic_error);
}

TEST(ArenaTest, OneRegionEmptyBeforeReset) {
  alignas(Region) uint8_t mem[sizeof(Region)];
  Region regions[] { mem };
  Arena arena(regions);

  ASSERT_EQ(0, arena.get_total_count());
  ASSERT_EQ(0, arena.get_free_count());

  ASSERT_THROW(arena.allocate(1), std::logic_error);
}

TEST(ArenaTest, OneRegionAfterReset) {
  alignas(Region) uint8_t mem[sizeof(Region)];
  Region regions[] { mem };
  Arena arena(regions);
  arena.reset();

  ASSERT_EQ(sizeof(Region), arena.get_total_count());
  ASSERT_EQ(0, arena.get_free_count());
}

TEST(ArenaTest, TwoRegionsAfterReset) {
  static_assert(sizeof(Region) < 64, "test assumption invalid");

  alignas(Region) uint8_t mem1[64];
  uint8_t mem2[32];

  Region regions[] { mem1, mem2 };
  Arena arena(regions);
  arena.reset();

  ASSERT_EQ(64 + 32, arena.get_total_count());
  ASSERT_EQ(64 + 32 - 2 * sizeof(Region), arena.get_free_count());
}


class ArenaTest_ThreeRegions : public ::testing::Test {
protected:
  alignas(Region) uint8_t region1[64];
  uint8_t region2[64];
  uint8_t region3[16];

  Region region_table[3] { {region1}, {region2}, {region3} };

  Arena arena{region_table};

  virtual void SetUp() {
    arena.reset();
  }

  static constexpr std::size_t
    expected_total_size = 64 + 64 + 16,
    region_count = 3;

  void exhaust() {
    static_assert(sizeof(void *) == 8, "unclear if this will work on 32-bit");
    arena.allocate(16);  // Should pretty much exhaust first region
    arena.allocate(62);  // There goes the second.
    arena.allocate(14);  // And the third.
  }
};

constexpr std::size_t ArenaTest_ThreeRegions::expected_total_size;
constexpr std::size_t ArenaTest_ThreeRegions::region_count;

TEST_F(ArenaTest_ThreeRegions, InitialState) {
  ASSERT_EQ(expected_total_size, arena.get_total_count());
  ASSERT_EQ(expected_total_size - region_count * sizeof(Region),
            arena.get_free_count());
}

TEST_F(ArenaTest_ThreeRegions, AllocateChangesFreeCount) {
  auto before = arena.get_free_count();
  arena.allocate(12);
  ASSERT_EQ(before - 12, arena.get_free_count());
}

TEST_F(ArenaTest_ThreeRegions, AllocatePrefersEarlierRegion) {
  auto p = arena.allocate(12);
  ASSERT_TRUE(region_table[0].contains(p));
}

TEST_F(ArenaTest_ThreeRegions, AllocateFindsFirstFit) {
  // The first region cannot accommodate a 64-byte request, since it contains
  // the arena state, which we know is >0 bytes.
  auto p = arena.allocate(64);
  ASSERT_TRUE(region_table[1].contains(p));
}

TEST_F(ArenaTest_ThreeRegions, AllocateAssertsIfTooBig) {
  ASSERT_THROW(arena.allocate(70), std::logic_error);
}

TEST_F(ArenaTest_ThreeRegions, AllocateAssertsIfExhausted) {
  exhaust();
  ASSERT_THROW(arena.allocate(14), std::logic_error);
}

TEST_F(ArenaTest_ThreeRegions, ResetAfterAllocate) {
  exhaust();
  arena.reset();
  ASSERT_EQ(expected_total_size, arena.get_total_count());
  ASSERT_EQ(expected_total_size - region_count * sizeof(Region),
            arena.get_free_count());
}

}  // namespace mem
}  // namespace etl
