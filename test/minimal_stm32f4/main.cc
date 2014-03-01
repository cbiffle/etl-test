#include "etl/armv7m/exception_table.h"
#include "etl/common/attribute_macros.h"

ETL_NORETURN void etl_armv7m_reset_handler() {
  while (1);
}

// Generate traps for all unused exception vectors.
#define ETL_ARMV7M_EXCEPTION(name) \
  ETL_NORETURN void etl_armv7m_ ## name ## _handler() { \
    while (1); \
  }

#define ETL_ARMV7M_EXCEPTION_RESERVED(n)

#include "etl/armv7m/exceptions.def"
