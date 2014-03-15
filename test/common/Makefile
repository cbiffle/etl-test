depth := ../..
products := types_test

types_test[type] := program
types_test[cc_files] := types_test.cc
types_test[cc_flags] := -std=gnu++0x -I$(depth)
types_test[libs] := \
  etl/common:common

include $(depth)/build/Makefile.rules
