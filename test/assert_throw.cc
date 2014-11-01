/*
 * Implements etl::assertion_failed to throw an exception, which is the
 * easiest way to integrate with gtest, even though it's the wrong way
 * to assert in general.
 */

#include <stdexcept>

#include "etl/assert.h"

namespace etl {

void assertion_failed(char const *, int, char const *, char const *) {
  throw std::logic_error("etl assertion failed");
}

}  // namespace etl
