#include <gtest/gtest.h>

#include "etl/data/range_ptr.h"

using etl::data::RangePtr;


/*******************************************************************************
 * Static tests - these pass if the test compiles.
 */

static int some_integers[12];
constexpr RangePtr<int> a_range(some_integers);
static_assert(a_range.base() == &some_integers[0],
              "RangePtr must capture array base address.");
static_assert(a_range.count() == sizeof(some_integers) / sizeof(int),
              "RangePtr must capture array count.");
static_assert(a_range.byte_length() == sizeof(some_integers),
              "byte_length() must parallel sizeof.");

/*******************************************************************************
 * Dynamic tests - these must be executed.
 */

TEST(RangePtr, Default) {
  RangePtr<int> empty;
  ASSERT_TRUE(empty.is_empty()) << "default RangePtr must be empty.";
  ASSERT_EQ(0, empty.count()) << "default RangePtr must have length of zero";
}

TEST(RangePtr, StaticArray) {
  int integers[12];
  RangePtr<int> range = integers;
  ASSERT_EQ(12, range.count()) << "RangePtr must adopt length of array.";
  ASSERT_EQ(&integers[0], range.base()) << "RangePtr must adopt base of array.";
}

TEST(RangePtr, ZeroLengthStaticArray) {
  // This one didn't work at first.
  int no_integers[0];
  RangePtr<int> range = no_integers;
  ASSERT_TRUE(range.is_empty());
}

TEST(RangePtr, ConversionToConst) {
  int integers[12];
  RangePtr<int> range = integers;
  RangePtr<int const> const_range = range;

  // Statically, this verifies that comparison of distinct types works.
  ASSERT_EQ(range, const_range);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}