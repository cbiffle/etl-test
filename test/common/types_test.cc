#include "etl/common/types.h"
#include "etl/common/type_traits.h"

#include <cstddef>

using etl::common::IsSame;
using etl::common::IsUnqualifiedIntegral;
using etl::common::IsSigned;
using etl::common::IsUnsigned;

/*
 * Sanity check on our explicitly-sized integer types.
 */
#define CHECK_INTEGER_TYPE(size) \
  static_assert(sizeof(etl::common::Int ## size) * 8 == size, \
                "The Int" #size " type must contain " #size " bits."); \
  static_assert(IsSigned<etl::common::Int ## size>::value, \
                "The Int" #size " type should be signed.")

CHECK_INTEGER_TYPE(8);
CHECK_INTEGER_TYPE(16);
CHECK_INTEGER_TYPE(32);
CHECK_INTEGER_TYPE(64);

#define CHECK_UNSIGNED_TYPE(size) \
  static_assert(sizeof(etl::common::UInt ## size) * 8 == size, \
                "The UInt" #size " type must contain " #size " bits."); \
  static_assert(IsUnsigned<etl::common::UInt ## size>::value, \
                "The UInt" #size " type should be unsigned.")

CHECK_UNSIGNED_TYPE(8);
CHECK_UNSIGNED_TYPE(16);
CHECK_UNSIGNED_TYPE(32);
CHECK_UNSIGNED_TYPE(64);

int main() { return 0; }
