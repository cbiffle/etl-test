#include <gtest/gtest.h>

#include "etl/error/flow.h"
#include "etl/type_traits.h"

using etl::IsSame;

/*
 * Error type for testing purposes.
 */

enum class TestError {
  ok = 0,
  failure_1 = 1,
  failure_2 = 2,
};

namespace etl {
  namespace error {
    template <>
    struct Strategy<TestError> {
      static constexpr bool is_error_type = true;

      static constexpr bool is_bad(TestError e) {
        return e != TestError::ok;
      }

      static constexpr TestError move_error(TestError e) { return e; }

      // This causes CHECK to become void, preventing use in expression
      // contexts.
      static void move_success(TestError) {}
    };
  }
}

TestError success_func() { return TestError::ok; }
TestError failure1_func() { return TestError::failure_1; }
TestError failure2_func() { return TestError::failure_2; }

TEST(CHECK, Basic) {
  TestError e = [] {
    ETL_CHECK(success_func());
    ETL_CHECK(failure1_func());  // ends execution
    ETL_CHECK(failure2_func());  // not reached

    throw std::exception();  // should not be reachable.
  }();
  ASSERT_EQ(TestError::failure_1, e);
}

TEST(CATCH, Basic) {
  TestError e = [] {
    ETL_CHECK(success_func());
    TestError x = ETL_CATCH(failure1_func());  // fails but returns
    ETL_CHECK(failure2_func());  // ends execution
    return x;  // not reached.
  }();
  ASSERT_EQ(TestError::failure_2, e);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
