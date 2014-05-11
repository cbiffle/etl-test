depth := ..
products := test_lib all_tests
subdirectories := *

test_lib[type] := library
test_lib[cc_files] := \
  size_test.cc \
  types_test.cc \
  type_traits_test.cc
test_lib[cc_flags] := -std=gnu++0x -I$(depth)
test_lib[libs] := \
  etl:etl

test_libs := \
  test:test_lib \
  test/biffield:tests \
  test/data:tests \
  test/error:tests

all_tests[type] := test
all_tests[cc_files] := main.cc
all_tests[cc_flags] := -std=gnu++0x -I$(depth)
all_tests[libs] := \
  $(test_libs) \
  3p/gtest:gtest
all_tests[whole_archive] := $(test_libs)

include $(depth)/build/Makefile.rules
