#include <stdexcept>

#include <gtest/gtest.h>

#include "etl/math/linear_transform.h"
#include "etl/math/matrix.h"
#include "etl/math/vector.h"

using namespace etl::math::linear_transform;

namespace etl {
namespace math {

/*
 * Tests for the linear transforms and associated stuff.
 */

static constexpr auto expected_scale = Matrix<3, 3, int> {
  {10, 0, 0},
  {0, 20, 0},
  {0, 0, 30},
};
static_assert(scale(Vec3f{10,20,30}) == expected_scale, "");

static_assert(scale(Vec3f{10,20,30}) * Vec3f{1, 2, 3}
        == Vec3f{10,40,90}, "");

}  // namespace math
}  // namespace etl
