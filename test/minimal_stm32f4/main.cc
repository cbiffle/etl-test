#include "etl/armv7m/exception_table.h"
#include "etl/attribute_macros.h"

#include "etl/armv7m/nvic.h"
#include "etl/armv7m/scb.h"

ETL_NORETURN void etl_armv7m_reset_handler() {
  etl::armv7m::scb.enable_faults();
  while (1);
}

// Generate traps for all unused exception vectors.
#define ETL_ARMV7M_EXCEPTION(name) \
  ETL_NORETURN void etl_armv7m_ ## name ## _handler() { \
    while (1); \
  }

#define ETL_ARMV7M_EXCEPTION_RESERVED(n)

#include "etl/armv7m/exceptions.def"
