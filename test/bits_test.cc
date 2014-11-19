#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "etl/bits.h"

/*
 * etl::bit_width
 */
static_assert(etl::bit_width<std::int8_t>() == 8, "");
static_assert(etl::bit_width<std::uint8_t>() == 8, "");

static_assert(etl::bit_width<std::int8_t>() >= 8, "");


/*
 * Uint, Int, UintLeast, IntLeast, UintFast, IntFast
 */

/*
 * These three templates, customized in terms of bit width, pair the signed and
 * unsigned int selection aliases for each variety.
 */

template <std::size_t N>
struct ExactAliases : std::integral_constant<std::size_t, N> {
  using Unsigned = etl::Uint<N>;
  using Signed = etl::Int<N>;
};

template <std::size_t N>
struct LeastAliases : std::integral_constant<std::size_t, N> {
  using Unsigned = etl::UintLeast<N>;
  using Signed = etl::IntLeast<N>;
};

template <std::size_t N>
struct FastAliases : std::integral_constant<std::size_t, N> {
  using Unsigned = etl::UintFast<N>;
  using Signed = etl::IntFast<N>;
};

/*
 * CheckExact verifies that a pair of aliases produces certain expected types,
 * and is also an implementation factor of the later checks.
 */
template <typename Aliases,  // Alias pair struct (above).
          typename Ts,  // Expected signed output type.
          typename Tu,  // Expected unsigned output type,
          std::size_t N = Aliases::value,  // Input bit width.
          typename Rs = typename Aliases::Signed,  // Actual signed type.
          typename Ru = typename Aliases::Unsigned  // Actual unsigned type.
>
struct CheckExact {
  static_assert(std::is_same<Rs, Ts>::value, "Signed type mismatch");
  static_assert(std::is_same<Ru, Tu>::value, "Unsigned type mismatch");
};

template struct CheckExact<ExactAliases< 8>, int8_t,  uint8_t>;
template struct CheckExact<ExactAliases<16>, int16_t, uint16_t>;
template struct CheckExact<ExactAliases<32>, int32_t, uint32_t>;
template struct CheckExact<ExactAliases<64>, int64_t, uint64_t>;

/*
 * CheckLeast combines CheckExact with tests at two bit sizes smaller than the
 * type's inherent size.
 *
 * For least and fast integers of size N (for N in 8, 16, 32, 64), N-1 and
 * N-(N/2-1) should see equivalent types.
 */
template <typename Aliases, typename Ts, typename Tu>
struct CheckLeast
  : CheckExact<Aliases, Ts, Tu>,
    CheckExact<Aliases, Ts, Tu, Aliases::value - 1>,
    CheckExact<Aliases, Ts, Tu, Aliases::value - (Aliases::value / 2 - 1)> {};

template struct CheckLeast<LeastAliases< 8>, int_least8_t,  uint_least8_t>;
template struct CheckLeast<LeastAliases<16>, int_least16_t, uint_least16_t>;
template struct CheckLeast<LeastAliases<32>, int_least32_t, uint_least32_t>;
template struct CheckLeast<LeastAliases<64>, int_least64_t, uint_least64_t>;

template struct CheckLeast<FastAliases< 8>, int_fast8_t,  uint_fast8_t>;
template struct CheckLeast<FastAliases<16>, int_fast16_t, uint_fast16_t>;
template struct CheckLeast<FastAliases<32>, int_fast32_t, uint_fast32_t>;
template struct CheckLeast<FastAliases<64>, int_fast64_t, uint_fast64_t>;


/*
 * Bitmasks
 */

static_assert(etl::bit_mask<0>() == 0, "");
static_assert(etl::bit_mask<1>() == 1, "");

static_assert(etl::bit_mask<4>() == 0b1111, "");
static_assert(etl::bit_mask<8>() == 0b11111111, "");
static_assert(etl::bit_mask<9>() == 0b111111111, "");

static_assert(etl::bit_mask<16>() == 0xFFFF, "");
static_assert(etl::bit_mask<20>() == 0xFFFFF, "");

static_assert(etl::bit_mask<32>() == 0xFFFFFFFF, "");
static_assert(etl::bit_mask<48>() == 0xFFFFFFFFFFFF, "");
static_assert(etl::bit_mask<64>() == 0xFFFFFFFFFFFFFFFF, "");
