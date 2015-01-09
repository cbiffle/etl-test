#include "etl/data/sort.h"

#include <gtest/gtest.h>

namespace etl {
namespace data {

using Data = RangePtr<std::uint8_t>;

template <typename T, size_t N>
RangePtr<T> array_range(T (&array)[N]) {
  return RangePtr<T>(array);
}

#define ASSERT_ARRAY_EQ(a, b) \
  { \
    static_assert(sizeof(a) == sizeof(b), \
                  "ASSERT_ARRAY_EQ used with different size arrays"); \
    auto a_ = array_range(a); \
    auto b_ = array_range(b); \
    for (unsigned i = 0; i < a_.count(); ++i) { \
      ASSERT_EQ(a_[i], b_[i]) << "Arrays unequal at index " << i; \
    } \
  }

template<typename T>
static bool less(T const & a, T const & b) {
  return a < b;
}

/*
 * Edge cases
 */

TEST(SortTest, EmptyRange) {
  Data d;
  qsort(d, less<std::uint8_t>);
}

TEST(SortTest, OneByte) {
  std::uint8_t data[] { 0 };

  qsort(array_range(data), less<std::uint8_t>);

  std::uint8_t const expected[] { 0 };
  ASSERT_ARRAY_EQ(expected, data);
}

TEST(SortTest, TwoBytesOrdered) {
  std::uint8_t data[] { 0, 1 };

  qsort(array_range(data), less<std::uint8_t>);

  std::uint8_t const expected[] { 0, 1 };
  ASSERT_ARRAY_EQ(expected, data);
}

TEST(SortTest, TwoBytesUnordered) {
  std::uint8_t data[] { 1, 0 };

  qsort(array_range(data), less<std::uint8_t>);

  std::uint8_t const expected[] { 0, 1 };
  ASSERT_ARRAY_EQ(expected, data);
}


/*
 * Slightly more interesting stuff
 */

TEST(SortTest, SortedEvenData) {
  std::uint8_t data[] { 0, 1, 2, 3, 4, 5, 6, 7 };

  qsort(array_range(data), less<std::uint8_t>);

  std::uint8_t const expected[] { 0, 1, 2, 3, 4, 5, 6, 7 };
  ASSERT_ARRAY_EQ(expected, data);
}

TEST(SortTest, UnsortedEvenData) {
  std::uint8_t data[] { 2, 1, 0, 3, 7, 5, 6, 4 };

  qsort(array_range(data), less<std::uint8_t>);

  std::uint8_t const expected[] { 0, 1, 2, 3, 4, 5, 6, 7 };
  ASSERT_ARRAY_EQ(expected, data);
}

TEST(SortTest, SortedOddData) {
  std::uint8_t data[] { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

  qsort(array_range(data), less<std::uint8_t>);

  std::uint8_t const expected[] { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  ASSERT_ARRAY_EQ(expected, data);
}

TEST(SortTest, UnsortedOddData) {
  std::uint8_t data[] { 2, 1, 0, 8, 3, 7, 5, 6, 4 };

  qsort(array_range(data), less<std::uint8_t>);

  std::uint8_t const expected[] { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  ASSERT_ARRAY_EQ(expected, data);
}

/*
 * Different compare functions.
 */

TEST(SortTest, ReversedLambda) {
  auto greater = [](std::uint8_t a, std::uint8_t b) { return a > b; };
  std::uint8_t data[] { 2, 1, 0, 8, 3, 7, 5, 6, 4 };
  qsort(array_range(data), greater);
  std::uint8_t const expected[] { 8, 7, 6, 5, 4, 3, 2, 1, 0 };
  ASSERT_ARRAY_EQ(expected, data);
}

}  // namespace data
}  // namespace etl
