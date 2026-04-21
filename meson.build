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
