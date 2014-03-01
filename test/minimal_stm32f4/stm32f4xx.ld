MEMORY {
  rom (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K
  ccm (rwx)     : ORIGIN = 0x10000000, LENGTH =   64K
}

EXTERN(_ZN3etl6armv7mL15exception_tableE)
ENTRY(etl_armv7m_reset_handler)

SECTIONS {
  .etl_armv7m_vector_table : {
    KEEP(*(.etl_armv7m_exception_table*))
  } >rom

  . = ALIGN(4);

  .text : {
    *(.text*)
    . = ALIGN(4);
  } >rom
}

PROVIDE(etl_armv7m_initial_stack_top = ORIGIN(ccm) + LENGTH(ccm));
