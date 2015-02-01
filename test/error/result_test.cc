#include <gtest/gtest.h>

#include "etl/error/result.h"
#include "etl/error/check.h"

#include "test/error/test_error.h"

namespace etl {
namespace error {

template <typename V>
using TResult = Result<TestError, V>;

static_assert(TResult<int>(left, TestError::failure_1).is_error(), "");

static TResult<int> success_func() {
  return {right, 1};
}

static TResult<int> failure_1_func() {
  return {left, TestError::failure_1};
}

TEST(Result, basic_success) {
  auto r = success_func();
  ASSERT_FALSE(r.is_error());
  ASSERT_EQ(1, r.ref());
}

TEST(Result, basic_failure) {
  auto r = failure_1_func();
  ASSERT_TRUE(r.is_error());
  ASSERT_EQ(TestError::failure_1, r.get_status());
}

}  // namespace error
}  // namespace etl
