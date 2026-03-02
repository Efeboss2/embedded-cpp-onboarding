gtest = subproject('gtest')
gtest_dep = gtest.get_variable('gtest_main_dep')

test_exe = executable('test_static_stack',
  'test_static_stack.cpp',
  include_directories: part1_inc,
  dependencies: gtest_dep)

test('static_stack_tests', test_exe)