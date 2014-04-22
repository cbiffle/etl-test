depth := ../..
products := maybe_test range_ptr_test

maybe_test[type] := test
maybe_test[cc_files] := maybe_test.cc
maybe_test[cc_flags] := -std=gnu++0x -I$(depth)
maybe_test[libs] := \
  etl/data:data \
  3p/gtest:gtest

range_ptr_test[type] := test
range_ptr_test[cc_files] := range_ptr_test.cc
range_ptr_test[cc_flags] := -std=gnu++0x -I$(depth)
range_ptr_test[libs] := \
  etl/data:data \
  3p/gtest:gtest

include $(depth)/build/Makefile.rules
