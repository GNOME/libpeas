libtesting_util_name = 'testing-util'
libtesting_util_string = '@0@-@1@'.format(libtesting_util_name, api_version)

libtesting_util_public_h = [
  'testing-util.h',
]

libtesting_util_c = [
  'testing-util.c',
]

libtesting_util_deps = [
  glib_dep,
  introspection_dep,
  libpeas_dep,
]

libtesting_util_c_args = [
  '-DBUILDDIR="@0@"'.format(builddir),
  '-DSRCDIR="@0@"'.format(srcdir),
  '-UG_DISABLE_ASSERT',
  '-UG_DISABLE_CAST_CHECKS',
]

libtesting_util_lib = library(
  libtesting_util_string,
  libtesting_util_c,
  include_directories: rootdir,
  dependencies: libtesting_util_deps,
  c_args: libtesting_util_c_args,
  install: false,
)

libtesting_util_dep = declare_dependency(
  link_with: libtesting_util_lib,
  include_directories: include_directories('.'),
  dependencies: libtesting_util_deps,
)
