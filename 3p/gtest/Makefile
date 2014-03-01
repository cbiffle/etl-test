depth := ../..
products := gtest

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

gtest[l_flags] := -lpthread
gtest[using_l_flags] := -lpthread

include $(depth)/build/Makefile.rules
