#include "etl/types.h"
#include "etl/type_traits.h"

#include <cstddef>
#include <climits>

using etl::IsSame;
using etl::IsUnqualifiedIntegral;
using etl::IsUnsigned;
using etl::MakeUnsigned;
using etl::MatchConst;
using etl::MatchVolatile;
using etl::MatchQualifiers;
using etl::RemoveConst;
using etl::RemoveVolatile;
using etl::RemoveQualifiers;

/*
 * Check determination of type bit width.
 */
static_assert(etl::char_bits == CHAR_BIT,
              "char_bits / CHAR_BIT mismatch");

/*
 * Check our qualifier-removing templates.
 */
#define CHECK_TYPE_DERIVE(src, op, dst) \
  static_assert(IsSame<dst, typename op<src>::Type>::value, \
                "Applying " #op " to " #src " should yield " #dst)

CHECK_TYPE_DERIVE(int, RemoveConst, int);
CHECK_TYPE_DERIVE(int const, RemoveConst, int);
CHECK_TYPE_DERIVE(int volatile, RemoveConst, int volatile);
CHECK_TYPE_DERIVE(int const volatile, RemoveConst, int volatile);

CHECK_TYPE_DERIVE(int, RemoveVolatile, int);
CHECK_TYPE_DERIVE(int const, RemoveVolatile, int const);
CHECK_TYPE_DERIVE(int volatile, RemoveVolatile, int);
CHECK_TYPE_DERIVE(int const volatile, RemoveVolatile, int const);

CHECK_TYPE_DERIVE(int, RemoveQualifiers, int);
CHECK_TYPE_DERIVE(int const, RemoveQualifiers, int);
CHECK_TYPE_DERIVE(int volatile, RemoveQualifiers, int);
CHECK_TYPE_DERIVE(int const volatile, RemoveQualifiers, int);

/*
 * Check our qualifier-transfer templates.
 */
#define CHECK_TYPE_TRANSFER(src1, src2, op, dst) \
  static_assert(IsSame<dst, typename op<src1, src2>::Type>::value, \
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



/*
 * Check the operation of MakeUnsigned on enum types.
 */
#define CHECK_MAKEUNSIGNED_ENUM(bits) \
  enum TestEnum ## bits : etl::Int ## bits {}; \
  typedef typename MakeUnsigned<TestEnum ## bits>::Type UnsignedEnum ## bits; \
  static_assert(IsUnsigned<UnsignedEnum ## bits>::value, \
                "MakeUnsigned on an enum should yield an unsigned type."); \
  static_assert(sizeof(UnsignedEnum ## bits) \
                    == sizeof(etl::UInt ## bits), \
                "MakeUnsigned on an enum must leave size unchanged.")

CHECK_MAKEUNSIGNED_ENUM(8);
CHECK_MAKEUNSIGNED_ENUM(16);
CHECK_MAKEUNSIGNED_ENUM(32);
CHECK_MAKEUNSIGNED_ENUM(64);
