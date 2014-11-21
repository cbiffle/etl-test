#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "etl/type_traits.h"

/*
 * Check our qualifier-transfer templates.
 */
#define CHECK_TYPE_TRANSFER(src1, src2, op, dst) \
  static_assert(std::is_same<dst, typename op<src1, src2>::Type>::value, \
                #op "<" #src1 ", " #src2 "> should yield " #dst)

CHECK_TYPE_TRANSFER(int, bool, etl::MatchConst, bool);
CHECK_TYPE_TRANSFER(int const, bool, etl::MatchConst, bool const);
CHECK_TYPE_TRANSFER(int volatile, bool, etl::MatchConst, bool);
CHECK_TYPE_TRANSFER(int const volatile, bool, etl::MatchConst, bool const);

CHECK_TYPE_TRANSFER(int, bool, etl::MatchVolatile, bool);
CHECK_TYPE_TRANSFER(int const, bool, etl::MatchVolatile, bool);
CHECK_TYPE_TRANSFER(int volatile, bool, etl::MatchVolatile, bool volatile);
CHECK_TYPE_TRANSFER(int const volatile, bool, etl::MatchVolatile,
                    bool volatile);

CHECK_TYPE_TRANSFER(int, bool, etl::MatchQualifiers, bool);
CHECK_TYPE_TRANSFER(int const, bool, etl::MatchQualifiers, bool const);
CHECK_TYPE_TRANSFER(int volatile, bool, etl::MatchQualifiers, bool volatile);
CHECK_TYPE_TRANSFER(int const volatile, bool, etl::MatchQualifiers,
                    bool const volatile);

#undef CHECK_TYPE_TRANSFER
