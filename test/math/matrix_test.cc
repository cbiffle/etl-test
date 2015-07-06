#include <stdexcept>

#include <gtest/gtest.h>

#include "etl/math/matrix.h"
#include "etl/math/affine.h"

namespace etl {
namespace math {

/*
 * Tests for the matrix templates.
 */

static constexpr auto id2 = Matrix<2, 2, int>::identity();
static_assert(id2.rows[0] == transpose(Vec2i{1, 0}), "");
static_assert(id2.rows[1] == transpose(Vec2i{0, 1}), "");

static_assert(id2 * Vec2i{3, 4} == Vec2i{3, 4}, "");

static constexpr auto a = Matrix<3, 2, int> {{
  {4, 8},
  {0, 2},
  {1, 6},
}};

static constexpr auto b = Matrix<2, 2, int> {{
  {5, 2},
  {9, 4},
}};

static constexpr auto expected_ab = Matrix<3, 2, int> {{
  {92, 40},
  {18, 8},
  {59, 26},
}};

static_assert(a * b == expected_ab, "");


static constexpr auto expected_translate = Matrix<4, 4, int> {{
  {1, 0, 0, 3},
  {0, 1, 0, 4},
  {0, 0, 1, 5},
  {0, 0, 0, 1},
}};

static_assert(translate(Vec3f{3, 4, 5}) == expected_translate, "");

static constexpr auto expected_trans_a = Matrix<2, 3, int> {{
  {4, 0, 1},
  {8, 2, 6},
}};

static_assert(transpose(a) == expected_trans_a, "");

}  // namespace math
}  // namespace etl
