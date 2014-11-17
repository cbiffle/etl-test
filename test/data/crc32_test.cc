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
