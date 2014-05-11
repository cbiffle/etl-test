gtest_case('tests',
  sources = [
    'size_test.cc',
    'types_test.cc',
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
