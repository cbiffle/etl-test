depth := ../..
products := tests

tests[type] := library
tests[cc_files] := flow_test.cc
tests[cc_flags] := -std=gnu++0x -I$(depth)
tests[libs] := \
  etl/error:error \
  3p/gtest:gtest

include $(depth)/build/Makefile.rules
