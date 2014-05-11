depth := ../..
products := tests

tests[type] := library
tests[cc_files] := \
  maybe_test.cc \
  range_ptr_test.cc
tests[cc_flags] := -std=gnu++0x -I$(depth)
tests[libs] := \
  etl/data:data \
  3p/gtest:gtest

include $(depth)/build/Makefile.rules
