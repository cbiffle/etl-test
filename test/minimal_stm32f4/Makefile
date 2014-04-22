depth := ../..
products := minimal_stm32f4

minimal_stm32f4[type] := program
minimal_stm32f4[arch] := stm32f4_hardfp
minimal_stm32f4[cc_files] := main.cc
minimal_stm32f4[cc_flags] := -std=gnu++0x -nostdinc -I$(depth)

minimal_stm32f4[libs] := \
  etl/armv7m:exception_table

minimal_stm32f4[whole_archive] := \
  etl/armv7m:exception_table

minimal_stm32f4[l_flags] = \
  -Wl,-Map=$(intermediate)/minimal_stm32f4.map \
  -Tstm32f4xx.ld \
  -nostdlib -nodefaultlibs

include $(depth)/build/Makefile.rules
