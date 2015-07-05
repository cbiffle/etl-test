c_library('assert_throw',
  sources = [
    'assert_throw.cc',
  ],
  deps = [
    '//etl',
  ],
)

gtest_case('tests',
  sources = [
    'bits_test.cc',
    'integer_sequence_test.cc',
    'invoke_test.cc',
    'non_null_test.cc',
    'scope_guard_test.cc',
    'type_list_test.cc',
    'type_traits_test.cc',
    'utility_test.cc',
  ],
  deps = [
    '//etl:etl',
  ],
)

gtest_runner('all_tests',
  environment = 'hosted-gcc',
  sources = [ 'main.cc' ],
  deps = [
    ':tests',
    '//test/biffield:tests',
    '//test/data:tests',
    '//test/error:tests',
    '//test/math:tests',
    '//test/mem:tests',
  ],
  extra = {
    'etl_config_use_toolchain_trig': True,
  },
)

gtest_runner('all_tests_clang',
  environment = 'hosted-clang',
  sources = [ 'main.cc' ],
  deps = [
    ':tests',
    '//test/biffield:tests',
    '//test/data:tests',
    '//test/error:tests',
    '//test/math:tests',
    '//test/mem:tests',
  ],
  extra = {
    'etl_config_use_toolchain_trig': True,
  },
)
