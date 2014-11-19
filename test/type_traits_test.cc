#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "etl/type_traits.h"

/*
 * Exercise Invoke, for if it fails, many other things will fail in unexpected
 * and obscure ways.
 */

static_assert(std::is_same<int, etl::Invoke<etl::TypeConstant<int>>>::value,
              "Invoke should trivially unwrap TypeConstant.");

static_assert(std::is_same<int,
                           etl::Invoke<std::conditional<true, int, bool>>
                           >::value,
              "Invoke should also unwrap std::conditional.");

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


/*
 * Size-based type selection.  All these tests make some assumptions about the
 * platform.  First, we assume that 8 and 16 bit types are available at all,
 * which isn't true for old RISCs and modern DSPs.  Second, we assume that these
 * types can be distinguished using sizeof, which tends to be false for the same
 * systems.
 */

static_assert(CHAR_BIT == 8, "These tests assume an eight-bit char.");
static_assert(sizeof(uint8_t) == 1, "uint8_t should occupy one char.");
static_assert(sizeof(uint16_t) == 2, "uint16_t should occupy two chars.");
static_assert(sizeof(uint32_t) == 4, "uint32_t should occupy four chars.");

template <std::size_t N>
using Ints = etl::SelectBySize<N, uint8_t, uint16_t, uint32_t>;

// Make sure the types come back out.
static_assert(std::is_same<Ints<1>, uint8_t>::value, "");
static_assert(std::is_same<Ints<2>, uint16_t>::value, "");
static_assert(std::is_same<Ints<4>, uint32_t>::value, "");

static_assert(std::is_same<int8_t, etl::SignedIntOfSize<1>>::value, "");
static_assert(std::is_same<int16_t, etl::SignedIntOfSize<2>>::value, "");
static_assert(std::is_same<int32_t, etl::SignedIntOfSize<4>>::value, "");
static_assert(std::is_same<int64_t, etl::SignedIntOfSize<8>>::value, "");

static_assert(std::is_same<uint8_t, etl::UnsignedIntOfSize<1>>::value, "");
static_assert(std::is_same<uint16_t, etl::UnsignedIntOfSize<2>>::value, "");
static_assert(std::is_same<uint32_t, etl::UnsignedIntOfSize<4>>::value, "");
static_assert(std::is_same<uint64_t, etl::UnsignedIntOfSize<8>>::value, "");
