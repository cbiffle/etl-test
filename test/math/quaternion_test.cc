#include <stdexcept>
#include <cmath>

#include <gtest/gtest.h>

#include "etl/math/quaternion.h"

namespace etl {
namespace math {

static constexpr auto tolerance = 1e-7f;

static constexpr auto q = quat(0.f, Vec3f{1, 2, 3});

static_assert(q.scalar == 0, "");
static_assert(q.vector == Vec3f{1, 2, 3}, "");
static_assert(conjugate(q).scalar == q.scalar, "");
static_assert(conjugate(q).vector == -Vec3f{1,2,3}, "");

TEST(UnitQuaternion, from_quaternion) {
  UnitQuaternion<float> u = normalized(q);
  ASSERT_FLOAT_EQ(1.f, norm(u));
}

TEST(UnitQuaternion, rotation_from_axis_angle_nonunit) {
  auto axis = Vec3f{0, 0, 1};       // Z pointing "out of screen"
  auto angle = float(M_PI) / 2;     // Quarter rotation.
  auto u = rotation(axis, angle);

  auto p = Vec3f{1, 0, 0};          // Start along X axis.
  auto r = Vec3f{0, 1, 0};          // Expect to end up along Y axis.

  ASSERT_NEAR(0.f, mag(r - u.rotate(p)), tolerance);
}

TEST(UnitQuaternion, rotation_from_axis_angle_unit) {
  auto axis = UVec3f::from_arbitrary(Vec3f{0, 0, 1});
  auto angle = float(M_PI) / 2;     // Quarter rotation.
  auto u = rotation(axis, angle);

  auto p = Vec3f{1, 0, 0};          // Start along X axis.
  auto r = Vec3f{0, 1, 0};          // Expect to end up along Y axis.

  ASSERT_NEAR(0.f, mag(r - u.rotate(p)), tolerance);
}

TEST(UnitQuaternion, rotation_from_vectors_nonunit) {
  constexpr auto start = Vec3f{1, 0, 0};
  constexpr auto end = Vec3f{0, 1, 0};
  auto u = rotation(start, end);

  ASSERT_NEAR(0.f, mag(end - u.rotate(start)), tolerance);
}

TEST(UnitQuaternion, rotation_from_vectors_unit) {
  constexpr auto start = UVec3f::from_arbitrary(Vec3f{1, 0, 0});
  constexpr auto end = UVec3f::from_arbitrary(Vec3f{0, 1, 0});
  auto u = rotation(start, end);

  ASSERT_NEAR(0.f, mag(end - u.rotate(start)), tolerance);
}


}  // namespace math
}  // namespace etl
