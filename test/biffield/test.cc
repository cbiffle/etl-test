#include <gtest/gtest.h>

#include "test/biffield/example.h"

Example ex;

/*
 * Compile-time test cases.
 */

// Value types must be literal types.
static constexpr auto foo_v = Example::foo_value_t()
                              .with_top(0xDEAD)
                              .with_bottom(0xBEEF);

static_assert(foo_v.get_all() == 0xDEADBEEF,
              "Fields must assemble as specified.");
static_assert(foo_v.get_all() == static_cast<uint32_t>(foo_v),
              "Cast operator must exist and return all bits.");

/*
 * Dynamic test cases.
 */

TEST(Biffield, Basic) {
  ex.write_foo(Example::foo_value_t()
               .with_top(0xDEAD)
               .with_bottom(0xBEEF));

  EXPECT_EQ(0xDEADBEEF, ex.read_foo().get_all());
}

TEST(Biffield, AtomicUpdate) {
  ex.write_foo(Example::foo_value_t()
               .with_top(0xDEAD)
               .with_bottom(0xBEEF));

  ex.update_foo([] (Example::foo_value_t v) {
      return v.with_top(0x600D);
  });

  EXPECT_EQ(0x600DBEEF, ex.read_foo().get_all());
}
