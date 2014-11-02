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
    'type_traits_test.cc',
  ],
  deps = [
    '//etl:etl',
  ],
)

gtest_runner('all_tests',
  environment = 'hosted',
  sources = [ 'main.cc' ],
  deps = [
    ':tests',
    '//test/biffield:tests',
    '//test/data:tests',
    '//test/error:tests',
  ],
)
