#include <gtest/gtest.h>

#include "test/biffield/example.h"

Example ex;

TEST(Biffield, Basic) {
  ex.write_foo(Example::foo_value_t()
               .with_top(0xDEAD)
               .with_bottom(0xBEEF));

  EXPECT_EQ(0xDEADBEEF, ex.read_foo().get_all());
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
