depth := ../..
products := flow_test

flow_test[type] := test
flow_test[cc_files] := flow_test.cc
flow_test[cc_flags] := -std=gnu++0x -I$(depth)
flow_test[libs] := \
  etl/error:error \
  3p/gtest:gtest

include $(depth)/build/Makefile.rules
