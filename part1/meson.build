project('part1', 'cpp',
  default_options: ['cpp_std=c++20', 'warning_level=3'])

add_global_arguments('-stdlib=libc++', language: 'cpp')
add_global_link_arguments('-stdlib=libc++', language: 'cpp')

subdir('src')
subdir('tests')