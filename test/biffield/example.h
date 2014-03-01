#ifndef TEST_BIFFIELD_EXAMPLE_H_
#define TEST_BIFFIELD_EXAMPLE_H_

#include <stdint.h>

struct Example {
  #define ETL_BFF_DEFINITION_FILE test/biffield/example.reg
  #include <etl/biffield/generate.h>
  #undef ETL_BFF_DEFINITION_FILE
};

#endif  // TEST_BIFFIELD_EXAMPLE_H_
