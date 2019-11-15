libbuiltin_configurable_name = 'builtin-configurable'

libbuiltin_configurable_public_h = [
  'builtin-configurable.h',
]

libbuiltin_configurable_c = [
  'builtin-configurable.c',
]

libbuiltin_configurable_plugin_data = [
  'builtin-configurable.plugin',
]

libbuiltin_configurable_deps = [
  glib_dep,
  gobject_dep,
  gmodule_dep,
  libpeas_dep,
  libpeas_gtk_dep,
]

libbuiltin_configurable_lib = shared_library(
  libbuiltin_configurable_name,
  libbuiltin_configurable_c,
  include_directories: rootdir,
  dependencies: libbuiltin_configurable_deps,
  install: false,
)

custom_target(
  'lib@0@-data'.format(libbuiltin_configurable_name),
  input: libbuiltin_configurable_plugin_data,
  output: libbuiltin_configurable_plugin_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
