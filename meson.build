project(
  'prism_enum',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++20'],
)

dependencies = [
]

inc = include_directories('include')

install_subdir('include', install_dir : get_option('includedir'), strip_directory : true)

prism_enum_dep = declare_dependency(
  include_directories : inc,
)

if not meson.is_subproject()
  catch2_dep = dependency('catch2-with-main', fallback: ['catch2', 'catch2_with_main_dep'])

  test_exe = executable(
    'prism_enum_test',
    'test/test_main.cpp',
    'test/test_edge_cases.cpp',
    dependencies : [prism_enum_dep, catch2_dep]
  )

  test('prism_enum_basic_test', test_exe)
endif
