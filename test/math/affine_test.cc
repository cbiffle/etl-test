#include <stdexcept>

#include <gtest/gtest.h>

#include "etl/math/affine_transform.h"
#include "etl/math/matrix.h"
#include "etl/math/vector.h"

using namespace etl::math::affine_transform;

namespace etl {
namespace math {

/*
 * Tests for the affine transforms and associated stuff.
 */

static_assert(augment(Vec3f{1,2,3}) == Vec4f{1,2,3,1}, "");
static_assert(project(Vec4f{2,4,6,2}) == Vec3f{1,2,3}, "");

static constexpr auto expected_translate = Matrix<4, 4, int> {
  {1, 0, 0, 3},
  {0, 1, 0, 4},
  {0, 0, 1, 5},
  {0, 0, 0, 1},
};

static_assert(translate(Vec3f{3, 4, 5}) == expected_translate, "");
static_assert(project(translate(Vec3f{3, 4, 5}) * augment(Vec3f{1, 2, 3}))
        == Vec3f{4,6,8}, "");

static constexpr auto expected_scale = Matrix<4, 4, int> {
  {10, 0, 0, 0},
  {0, 20, 0, 0},
  {0, 0, 30, 0},
  {0, 0, 0,  1},
};
static_assert(scale(Vec3f{10,20,30}) == expected_scale, "");

static_assert(project(scale(Vec3f{10,20,30}) * augment(Vec3f{1, 2, 3}))
        == Vec3f{10,40,90}, "");

}  // namespace math
}  // namespace etl
