depth := ../..
products := gtest pthread

gtest[type] := library

gtest[cc_files] := upstream/src/gtest-all.cc

gtest[cc_flags] := \
  -isystem upstream/include \
  -isystem upstream \
  -DGTEST_OS_LINUX \
  -Wno-undef

gtest[using_c_flags] := \
  -isystem $(shell pwd)/upstream/include \
  -DGTEST_OS_LINUX

gtest[libs] := 3p/gtest:pthread

pthread[type] := library
pthread[using_l_flags] := -lpthread

include $(depth)/build/Makefile.rules
