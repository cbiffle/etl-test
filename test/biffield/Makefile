depth := ../..
products := tests

tests[type] := library
tests[cc_files] := test.cc
tests[cc_flags] := -std=gnu++0x -I$(depth)
tests[libs] := \
  etl/biffield:biffield \
  3p/gtest:gtest

include $(depth)/build/Makefile.rules
