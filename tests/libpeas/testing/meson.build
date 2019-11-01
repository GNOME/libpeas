libpeas_testing_name = 'libpeas-testing'
libpeas_testing_string = '@0@-@1@'.format(libpeas_testing_name, api_version)

libpeas_testing_public_h = [
  'testing.h',
  'testing-extension.h',
]

libpeas_testing_c = [
  'testing.c',
  'testing-extension.c',
]

libpeas_testing_deps = [
  glib_dep,
  introspection_dep,
  libpeas_dep,
  libtesting_util_dep,
  libintrospection_dep,
]

libpeas_testing_c_args = [
  '-DHAVE_CONFIG_H',
  '-DBUILDDIR="@0@"'.format(builddir),
  '-DSRCDIR="@0@"'.format(srcdir),
]

libpeas_testing_lib = library(
  libpeas_testing_string,
  libpeas_testing_c,
  include_directories: rootdir,
  dependencies: libpeas_testing_deps,
  c_args: libpeas_testing_c_args,
  install: false,
)

libpeas_testing_dep = declare_dependency(
  link_with: libpeas_testing_lib,
  include_directories: include_directories('.'),
  dependencies: libpeas_testing_deps,
)
