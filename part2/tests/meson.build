gtest_dep = dependency('gtest', required : true)
gtest_main_dep = dependency('gtest_main', required : true) 


test_exe = executable('test_static_stack',
    sources: ['test_static_stack.cpp'],
    include_directories: include_directories('../src'),
    dependencies: [gtest_dep, gtest_main_dep]
)

test('part2_tests', test_exe)