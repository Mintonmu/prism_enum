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

prism_enum_lib = static_library(
  'prism_enum',
  [
    'src/prism_enum.cpp',
    'src/prism_impl.cpp',
  ],
  include_directories : inc,
  install : true,
  dependencies : dependencies,
)

prism_enum_dep = declare_dependency(
  link_with : prism_enum_lib,
  include_directories : inc,
)
