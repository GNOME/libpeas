libbuiltin_name = 'builtin'

libbuiltin_public_h = [
  'builtin-plugin.h',
]

libbuiltin_c = [
  'builtin-plugin.c',
]

libbuiltin_plugin_data = [
  'builtin.plugin',
]

libbuiltin_plugin_deps = [
  glib_dep,
  gobject_dep,
  libpeas_dep,
  libintrospection_dep,
]

libbuiltin_lib = shared_library(
  libbuiltin_name,
  libbuiltin_c,
  dependencies: libbuiltin_plugin_deps,
  install: false,
)

custom_target(
  'lib@0@-data'.format(libbuiltin_name),
  input: libbuiltin_plugin_data,
  output: libbuiltin_plugin_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
