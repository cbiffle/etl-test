#include <gtest/gtest.h>

#include "etl/error/flow.h"

#include "test_error.h"

static TestError success_func() { return TestError::ok; }
static TestError failure1_func() { return TestError::failure_1; }
static TestError failure2_func() { return TestError::failure_2; }

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
