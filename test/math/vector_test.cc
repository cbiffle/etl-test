#include <stdexcept>

#include <gtest/gtest.h>

#include "etl/math/vector.h"

namespace etl {
namespace math {

/*
 * Tests for the vector templates.
 *
 * You'll see a repeating pattern in these tests: we check most properties for
 * vectors of 2, 3, 4, and 5 components.  This is because there are template
 * specializations for the 2-4 case, and the 5 case is an example of the
 * fallback specialization for n-vecs.
 */

/*******************************************************************************
 * Field assignment and naming tests.
 *
 * These are written as static tests to ensure that vector construction and
 * deconstruction are constexpr.
 */

namespace _field_name_tests {
  static constexpr auto v2 = Vec2f{1.f, 2.f};
  static_assert(v2.x == 1.f, "");
  static_assert(v2.y == 2.f, "");

  static constexpr auto v3 = Vec3f{1.f, 2.f, 3.f};
  static_assert(v3.x == 1.f, "");
  static_assert(v3.y == 2.f, "");
  static_assert(v3.z == 3.f, "");

  static constexpr auto v4 = Vec4f{1.f, 2.f, 3.f, 4.f};
  static_assert(v4.x == 1.f, "");
  static_assert(v4.y == 2.f, "");
  static_assert(v4.z == 3.f, "");
  static_assert(v4.w == 4.f, "");

  static constexpr auto v5 = Vector<5, float>{1.f, 2, 3.f, 4.f, 5.f};
  static_assert(v5.get<0>() == 1.f, "");
  static_assert(v5.get<1>() == 2.f, "");
  static_assert(v5.get<2>() == 3.f, "");
  static_assert(v5.get<3>() == 4.f, "");
  static_assert(v5.get<4>() == 5.f, "");

}  // namespace _field_name_tests


/*******************************************************************************
 * Element repetition constructor tests.
 *
 * These are written as static tests to ensure that vector construction and
 * deconstruction are constexpr.
 */
namespace _repeat_tests {
  static constexpr auto v2_repeat = Vec2f{1.f};
  static_assert(v2_repeat.x == 1.f, "");
  static_assert(v2_repeat.y == 1.f, "");

  static constexpr auto v3_repeat = Vec3f{1.f};
  static_assert(v3_repeat.x == 1.f, "");
  static_assert(v3_repeat.y == 1.f, "");
  static_assert(v3_repeat.z == 1.f, "");

  static constexpr auto v4_repeat = Vec4f{1.f};
  static_assert(v4_repeat.x == 1.f, "");
  static_assert(v4_repeat.y == 1.f, "");
  static_assert(v4_repeat.z == 1.f, "");
  static_assert(v4_repeat.w == 1.f, "");

  static constexpr auto v5_repeat = Vector<5, float>{1.f};
  static_assert(v5_repeat.get<0>() == 1.f, "");
  static_assert(v5_repeat.get<1>() == 1.f, "");
  static_assert(v5_repeat.get<2>() == 1.f, "");
  static_assert(v5_repeat.get<3>() == 1.f, "");
  static_assert(v5_repeat.get<4>() == 1.f, "");

}  // namespace _repeat_tests


/*******************************************************************************
 * Shuffle tests.
 *
 * These are written as static tests to ensure that vector construction and
 * deconstruction are constexpr.
 *
 * Since shuffling is implemented in the top-level template, we don't need to
 * exhaustive evaluate the specializations.
 */
namespace _shuffle_tests {
  static constexpr auto v3 = Vec3f{1.f, 2.f, 3.f};
  static_assert(v3.get<0, 1, 2>() == v3, "");
  static_assert(v3.get<0>() == 1.f, "");
  static_assert(v3.get<0, 1>() == Vec2f{1.f, 2.f}, "");
  static_assert(v3.get<1, 1, 1, 1>() == Vec4f{2.f}, "");
}  // namespace _shuffle_tests


/*******************************************************************************
 * Horizontal combinator template tests.
 *
 * This template is covered by its other uses, but this provides more precise
 * reporting should the template get broken.
 *
 * Note that since horizontal is size-independent, we don't really need to test
 * exhaustively.
 */

TEST(Vector, horizontal) {
  ASSERT_EQ(3.f,
      horizontal(Vec2f { 1.f, 2.f }, [](float a, float b) { return a + b; }));
}


/*******************************************************************************
 * Parallel combinator template tests.
 *
 * This template is covered by its other uses, but this provides more precise
 * reporting should the template get broken.
 *
 * Note that since parallel is size-independent, we don't really need to test
 * exhaustively.
 */

TEST(Vector, parallel_unary) {
  ASSERT_EQ((Vec3f{-1.f, -2.f, -3.f}),
      parallel(Vec3f { 1.f, 2.f, 3.f }, [](float a) { return -a; }));
}

TEST(Vector, parallel_binary) {
  ASSERT_EQ((Vec3f{5.f, 7.f, 9.f}),
      parallel(Vec3f{1.f, 2.f, 3.f},
               Vec3f{4.f, 5.f, 6.f},
               [](float a, float b) { return a + b; }));
}


/*******************************************************************************
 * Equality/inequality.  The equality operator is widely instantiated in this
 * file, so we focus on inequality here.
 */

TEST(Vector, ne3) {
  ASSERT_NE((Vec3f { 1.f, 1.f, 2.f }), (Vec3f { 1.f, 1.f, 1.f }));
  ASSERT_FALSE((Vec3f { 1.f, 1.f, 1.f } != Vec3f { 1.f, 1.f, 1.f }));
}


/*******************************************************************************
 * Component-wise arithmetic operators.  Since these are implemented in terms
 * of the binary template, which is exercised above, we don't need to
 * exhaustively test for all vector sizes.
 */

TEST(Vector, neg3) {
  ASSERT_EQ(
      (Vec3f{-1.f, -2.f, -3.f}),
      (-Vec3f{1.f, 2.f, 3.f}));
}

TEST(Vector, add3) {
  ASSERT_EQ(
      (Vec3f{5.f, 7.f, 9.f}),
      (Vec3f{1.f, 2.f, 3.f} + Vec3f{4.f, 5.f, 6.f}));
}

TEST(Vector, add_eq3) {
  auto v = Vec3f{1.f, 2.f, 3.f};
  v += Vec3f{4.f, 5.f, 6.f};
  ASSERT_EQ((Vec3f{5.f, 7.f, 9.f}), v);
}

TEST(Vector, sub3) {
  ASSERT_EQ(
      (Vec3f{3.f, 3.f, 3.f}),
      (Vec3f{4.f, 5.f, 6.f} - Vec3f{1.f, 2.f, 3.f}));
}

TEST(Vector, sub_eq3) {
  auto v = Vec3f{1.f, 2.f, 3.f};
  v -= Vec3f{4.f, 5.f, 6.f};
  ASSERT_EQ((Vec3f{-3.f, -3.f, -3.f}), v);
}

TEST(Vector, mul3) {
  ASSERT_EQ(
      (Vec3f{4.f, 10.f, 18.f}),
      (Vec3f{4.f, 5.f, 6.f} * Vec3f{1.f, 2.f, 3.f}));
}

TEST(Vector, mul_eq3) {
  auto v = Vec3f{1.f, 2.f, 3.f};
  v *= Vec3f{4.f, 5.f, 6.f};
  ASSERT_EQ((Vec3f{4.f, 10.f, 18.f}), v);
}

TEST(Vector, div3) {
  ASSERT_EQ(
      (Vec3f{4.f/1.f, 5.f/2.f, 6.f/3.f}),
      (Vec3f{4.f, 5.f, 6.f} / Vec3f{1.f, 2.f, 3.f}));
}

TEST(Vector, div_eq3) {
  auto v = Vec3f{1.f, 2.f, 3.f};
  v /= Vec3f{4.f, 5.f, 6.f};
  ASSERT_EQ((Vec3f{1/4.f, 2/5.f, 3/6.f}), v);
}


/*******************************************************************************
 * Vector-specific operations, i.e. not simple lifts of C operators.
 */

TEST(Vector, transpose) {
  auto col = Vec3f { 1.f, 1.f, 1.f };
  auto row = transpose(col);
  ASSERT_EQ(Orient::row, Orient(decltype(row)::orient));
  auto col2 = transpose(row);
  ASSERT_EQ(Orient::col, Orient(decltype(col2)::orient));
  ASSERT_EQ(col, col2);
}

TEST(Vector, dot) {
  ASSERT_EQ(1 * 4 + 2 * 5 + 3 * 6,
            dot(Vec3f{1.f, 2.f, 3.f}, Vec3f{4.f, 5.f, 6.f}));
}

TEST(Vector, cross) {
  ASSERT_EQ((Vec3f{1.f, 0.f, 0.f}),
            cross(Vec3f{0.f, 1.f, 0.f}, Vec3f{0.f, 0.f, 1.f}));
}

}  // namespace math
}  // namespace etl
