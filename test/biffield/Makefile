depth := ../..
products := test

test[type] := gtest
test[cc_files] := test.cc
test[cc_flags] := -std=gnu++0x -I$(depth)
test[libs] := \
  etl/biffield:biffield \
  3p/gtest:gtest

include $(depth)/build/Makefile.rules
