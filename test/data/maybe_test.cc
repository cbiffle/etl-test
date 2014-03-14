#include <gtest/gtest.h>

#include "etl/data/maybe.h"

using etl::data::Maybe;
using etl::data::nothing;

TEST(Maybe, Basic) {
  Maybe<int> m(nothing);

  ASSERT_FALSE(!!m);

  m = 42;

  ASSERT_TRUE(!!m);
  ASSERT_EQ(42, m.ref());

  m = nothing;

  ASSERT_FALSE(!!m);
}


TEST(Maybe, DefaultCtorNotRequired) {
  class NoDefaultCtor {
   public:
    NoDefaultCtor(int x) : _x(x) {}

   private:
    int _x;
  };

  Maybe<NoDefaultCtor> x(nothing);
  ASSERT_FALSE(!!x);
  x = NoDefaultCtor(12);
  ASSERT_TRUE(!!x);
}

TEST(Maybe, ValueNotConstructedUnlessRequested) {
  class ExplodingDefaultCtor {
   public:
    ExplodingDefaultCtor() { EXPECT_FALSE(true); }
  };

  Maybe<ExplodingDefaultCtor> x(nothing);
  ASSERT_FALSE(!!x);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
