#include "etl/data/crc32.h"

#include <gtest/gtest.h>

using etl::data::crc32;

TEST(Crc32, CatalogueCheck) {
  /*
   * This checks the implementation against the value noted in the Catalogue of
   * Parametrised CRC Algorithms:
   *  http://reveng.sourceforge.net/crc-catalogue/17plus.htm#crc.cat.crc-32
   */
  std::uint8_t const input[] { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

  auto result = crc32(input);

  ASSERT_EQ(0xcbf43926, result);
}

TEST(Crc32, CatalogueCheckTwoParts) {
  /*
   * This checks the implementation against the value noted in the Catalogue of
   * Parametrised CRC Algorithms:
   *  http://reveng.sourceforge.net/crc-catalogue/17plus.htm#crc.cat.crc-32
   *
   * We do it in two chunks to verify chaining behavior.
   */
  std::uint8_t const input1[] { '1', '2', '3', '4' };
  std::uint8_t const input2[] { '5', '6', '7', '8', '9' };

  auto result = crc32(input2, crc32(input1));

  ASSERT_EQ(0xcbf43926, result);
}
