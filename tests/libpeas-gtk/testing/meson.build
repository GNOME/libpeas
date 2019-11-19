libpeas_gtk_testing_name = 'libpeas-gtk-testing'
libpeas_gtk_testing_string = '@0@-@1@'.format(libpeas_gtk_testing_name, api_version)

libpeas_gtk_testing_public_h = [
  'testing.h',
]

libpeas_gtk_testing_c = [
  'testing.c',
]

libpeas_gtk_testing_deps = [
  glib_dep,
  introspection_dep,
  libpeas_dep,
  libpeas_gtk_dep,
  libtesting_util_dep,
]

libpeas_gtk_testing_c_args = [
  '-DHAVE_CONFIG_H',
  '-DBUILDDIR="@0@"'.format(builddir),
  '-DSRCDIR="@0@"'.format(srcdir),
]

libpeas_gtk_testing_lib = library(
  libpeas_gtk_testing_string,
  libpeas_gtk_testing_c,
  include_directories: rootdir,
  dependencies: libpeas_gtk_testing_deps,
  c_args: libpeas_gtk_testing_c_args,
  install: false,
)

libpeas_gtk_testing_dep = declare_dependency(
  link_with: libpeas_gtk_testing_lib,
  include_directories: include_directories('.'),
  dependencies: libpeas_gtk_testing_deps,
)
