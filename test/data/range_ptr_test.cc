#include <stdexcept>
#include <type_traits>

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

static_assert(a_range.first(3).base() == &some_integers[0],
              "first(n) must not change base address.");
static_assert(a_range.first(3).count() == 3,
              "first(n) must yield n elements.");

static_assert(std::is_literal_type<RangePtr<int>>::value,
              "RangePtr should be a literal type.");

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

TEST(RangePtr, SliceBoundaries) {
  int integers[12];
  RangePtr<int> range = integers;
  auto slice = range.slice(1, 6);

  ASSERT_EQ(5, slice.count()) << "Slice should take indices, not a length.";
}

/*******************************************************************************
 * Policy tests
 */

template <typename T>
using RangePtrX = RangePtr<T, etl::data::AssertRangeCheckPolicy>;

constexpr RangePtrX<int> checked_range = some_integers;

static_assert(checked_range.base() == &some_integers[0],
              "RangePtr must capture array base address.");
static_assert(checked_range.count() == sizeof(some_integers) / sizeof(int),
              "RangePtr must capture array count.");
static_assert(checked_range.byte_length() == sizeof(some_integers),
              "byte_length() must parallel sizeof.");

static_assert(checked_range.first(3).base() == &some_integers[0],
              "first(n) must not change base address.");
static_assert(checked_range.first(3).count() == 3,
              "first(n) must yield n elements.");

static_assert(std::is_literal_type<RangePtrX<int>>::value,
              "RangePtrX should be a literal type.");

// Some future test framework should include a way to assert that something
// doesn't compile.  This shouldn't (and doesn't at this writing):
// constexpr int * foo = &checked_range[12];

TEST(RangePtr, CheckedAccess) {
  int integers[12];
  RangePtrX<int> range = integers;

  integers[0] = 42;
  ASSERT_EQ(42, range[0]);
  ASSERT_THROW(range[12], std::logic_error);
}
