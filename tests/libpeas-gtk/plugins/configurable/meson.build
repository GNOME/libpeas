libconfigurable_name = 'configurable'

libconfigurable_public_h = [
  'configurable-plugin.h',
]

libconfigurable_c = [
  'configurable-plugin.c',
]

libconfigurable_plugin_data = [
  'configurable.plugin',
]

libconfigurable_deps = [
  glib_dep,
  gobject_dep,
  gmodule_dep,
  libpeas_dep,
  libpeas_gtk_dep,
]

libconfigurable_lib = shared_library(
  libconfigurable_name,
  libconfigurable_c,
  include_directories: rootdir,
  dependencies: libconfigurable_deps,
  install: false,
)

custom_target(
  'lib@0@-data'.format(libconfigurable_name),
  input: libconfigurable_plugin_data,
  output: libconfigurable_plugin_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
