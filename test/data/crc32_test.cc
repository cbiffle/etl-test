#include "etl/data/crc32.h"
#include "etl/data/crc32_impl.h"

#include <gtest/gtest.h>

namespace etl {
namespace data {

/*
 * We want to evaluate the Crc32 implementation at all supported table sizes.
 * GTest does not support parameterizing a test case by a *compile-time* value,
 * only a runtime value or a compile-time type.
 *
 * Fortunately, we can lift values to types.  std::integral_constant to the
 * rescue!
 */

template <typename N>
class Crc32Test : public ::testing::Test,
                  public Crc32Table<N::value> {};

using TestTypes = ::testing::Types<
  std::integral_constant<std::size_t, 1>,
  std::integral_constant<std::size_t, 2>,
  std::integral_constant<std::size_t, 4>,
  std::integral_constant<std::size_t, 8>
>;
TYPED_TEST_CASE(Crc32Test, TestTypes);

using Data = RangePtr<std::uint8_t const>;

/*
 * The CatalogueChecks below test the implementation against the value noted in
 * The Catalogue of Parametrised CRC Algorithms:
 *  http://reveng.sourceforge.net/crc-catalogue/17plus.htm#crc.cat.crc-32
 */

static constexpr std::uint8_t catalogue_vector[] {
  '1', '2', '3', '4', '5', '6', '7', '8', '9'
};
static constexpr std::uint32_t catalogue_result = 0xcbf43926;

TYPED_TEST(Crc32Test, CatalogueCheckAllAtOnce) {
  ASSERT_EQ(catalogue_result, this->process(catalogue_vector));
}

TYPED_TEST(Crc32Test, CatalogueCheckCharAtATime) {
  Data v = catalogue_vector;
  std::uint32_t result = 0;
  for (unsigned i = 0; i < v.count(); ++i) {
    result = this->process(v.slice(i, i + 1), result);
  }

  ASSERT_EQ(catalogue_result, result);
}

TYPED_TEST(Crc32Test, CatalogueCheckTwoChunks) {
  Data v = catalogue_vector;
  ASSERT_EQ(catalogue_result,
      this->process(v.tail_from(4), this->process(v.slice(0, 4))));
}

}  // namespace data
}  // namespace etl
