#ifndef TEST_ERROR_H
#define TEST_ERROR_H

#include "etl/error/strategy.h"

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
    struct Strategy<TestError>
      : public TraditionalEnumStrategy<TestError, TestError::ok> {};
  }
}

#endif  // TEST_ERROR_H
