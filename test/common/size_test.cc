#include "etl/common/size.h"
#include "etl/common/type_traits.h"

#include <cstddef>

using etl::common::IsSame;
using etl::common::IsUnqualifiedIntegral;
using etl::common::IsUnsigned;
using etl::common::MakeUnsigned;
using etl::common::Size;

/*
 * Since Size is intended to parallel size_t, we test the following properties
 * of size_t:
 *
 *  "std::size_t is the unsigned integer type of the result of the sizeof
 *   operator and the alignof operator."
 */
static_assert(IsUnqualifiedIntegral<Size>::value, "Size must be integral.");
static_assert(IsUnsigned<Size>::value, "Size must be unsigned.");
static_assert(IsSame<Size, decltype(sizeof(char(0)))>::value,
              "Size must be the type of sizeof.");
static_assert(IsSame<Size, decltype(alignof(char(0)))>::value,
              "Size must be the type of alignof.");

int main() { return 0; }
