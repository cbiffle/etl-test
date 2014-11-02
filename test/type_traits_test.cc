#include <type_traits>

#include "etl/type_traits.h"

using etl::MatchConst;
using etl::MatchVolatile;
using etl::MatchQualifiers;

/*
 * Check our qualifier-transfer templates.
 */
#define CHECK_TYPE_TRANSFER(src1, src2, op, dst) \
  static_assert(std::is_same<dst, typename op<src1, src2>::Type>::value, \
                #op "<" #src1 ", " #src2 "> should yield " #dst)

CHECK_TYPE_TRANSFER(int, bool, MatchConst, bool);
CHECK_TYPE_TRANSFER(int const, bool, MatchConst, bool const);
CHECK_TYPE_TRANSFER(int volatile, bool, MatchConst, bool);
CHECK_TYPE_TRANSFER(int const volatile, bool, MatchConst, bool const);

CHECK_TYPE_TRANSFER(int, bool, MatchVolatile, bool);
CHECK_TYPE_TRANSFER(int const, bool, MatchVolatile, bool);
CHECK_TYPE_TRANSFER(int volatile, bool, MatchVolatile, bool volatile);
CHECK_TYPE_TRANSFER(int const volatile, bool, MatchVolatile, bool volatile);

CHECK_TYPE_TRANSFER(int, bool, MatchQualifiers, bool);
CHECK_TYPE_TRANSFER(int const, bool, MatchQualifiers, bool const);
CHECK_TYPE_TRANSFER(int volatile, bool, MatchQualifiers, bool volatile);
CHECK_TYPE_TRANSFER(int const volatile, bool, MatchQualifiers,
                    bool const volatile);
