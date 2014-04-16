depth := ..
products := subdirs size_test types_test type_traits_test

size_test[type] := program
size_test[cc_files] := size_test.cc
size_test[cc_flags] := -std=gnu++0x -I$(depth)
size_test[libs] := \
  etl:etl

types_test[type] := program
types_test[cc_files] := types_test.cc
types_test[cc_flags] := -std=gnu++0x -I$(depth)
types_test[libs] := \
  etl:etl

type_traits_test[type] := program
type_traits_test[cc_files] := type_traits_test.cc
type_traits_test[cc_flags] := -std=gnu++0x -I$(depth)
type_traits_test[libs] := \
  etl:etl


subdirs[type] := subdirectories

include $(depth)/build/Makefile.rules
