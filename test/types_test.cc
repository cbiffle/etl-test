#include "etl/types.h"
#include "etl/type_traits.h"

#include <cstddef>

using etl::IsSame;
using etl::IsUnqualifiedIntegral;
using etl::IsSigned;
using etl::IsUnsigned;

/*
 * Sanity check on our explicitly-sized integer types.
 */
#define CHECK_INTEGER_TYPE(size) \
  static_assert(sizeof(etl::Int ## size) * 8 == size, \
                "The Int" #size " type must contain " #size " bits."); \
  static_assert(IsSigned<etl::Int ## size>::value, \
                "The Int" #size " type should be signed.")

CHECK_INTEGER_TYPE(8);
CHECK_INTEGER_TYPE(16);
CHECK_INTEGER_TYPE(32);
CHECK_INTEGER_TYPE(64);

#define CHECK_UNSIGNED_TYPE(size) \
  static_assert(sizeof(etl::UInt ## size) * 8 == size, \
                "The UInt" #size " type must contain " #size " bits."); \
  static_assert(IsUnsigned<etl::UInt ## size>::value, \
                "The UInt" #size " type should be unsigned.")

CHECK_UNSIGNED_TYPE(8);
CHECK_UNSIGNED_TYPE(16);
CHECK_UNSIGNED_TYPE(32);
CHECK_UNSIGNED_TYPE(64);
