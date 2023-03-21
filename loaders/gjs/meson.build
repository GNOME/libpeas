add_languages('cpp')

cxx = meson.get_compiler('cpp')

gjs_loader_name = 'gjsloader'

gjs_loader_c = files(
  'peas-plugin-loader-gjs.cpp',
)

gjs_loader_deps = [
  libpeas_dep,
  gjs_dep,
]

gjs_loader_c_args = [
  '-DPEAS_LOCALEDIR="@0@"'.format(localedir),
]

gjs_loader_cpp_args = []
test_cpp_args = [
  '-Wno-missing-field-initializers',
  '-Wno-unused-parameter',
]
foreach arg: test_cpp_args
  if cxx.has_multi_arguments(arg)
    gjs_loader_cpp_args += arg
  endif
endforeach


gjs_loader_sha = shared_module(
  gjs_loader_name,
  gjs_loader_c,
  include_directories: rootdir,
  dependencies: gjs_loader_deps,
  c_args: project_c_args + gjs_loader_c_args,
  cpp_args: gjs_loader_cpp_args,
  install: true,
  install_dir: join_paths(
    pkglibdir,
    'loaders',
  ),
  name_suffix: module_suffix,
  gnu_symbol_visibility: 'hidden'
)
