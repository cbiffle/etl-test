#include "etl/utility.h"

#include <gtest/gtest.h>

namespace etl {

TEST(SwapTest, Primitive) {
  int a = 0xA;
  int b = 0xB;

  swap(a, b);

  ASSERT_EQ(0xB, a);
  ASSERT_EQ(0xA, b);
}

TEST(SwapTest, PrimitiveArray) {
  int a[] = { 0xA, 0xA, 0xA };
  int b[] = { 0xB, 0xB, 0xB };

  swap(a, b);

  for (int av : a) ASSERT_EQ(0xB, av);
  for (int bv : b) ASSERT_EQ(0xA, bv);
}

}  // namespace etl
