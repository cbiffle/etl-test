#include <gtest/gtest.h>

#include "etl/data/maybe.h"

using etl::data::Maybe;

TEST(Maybe, Basic) {
  Maybe<int> m;

  ASSERT_FALSE(!!m);

  m = 42;

  ASSERT_TRUE(!!m);
  ASSERT_EQ(42, *m);
}

enum ObjectState {
  zero = 0,
  constructed = 1,
  destroyed = 2,
};

class NoDefaultCtor {
 public:
  NoDefaultCtor(int x) : _x(x) {}

 private:
  int _x;
};

TEST(Maybe, DefaultCtorNotRequired) {
  Maybe<NoDefaultCtor> x;
  ASSERT_FALSE(!!x);
  x = NoDefaultCtor(12);
  ASSERT_TRUE(!!x);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
