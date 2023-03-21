libloadable_name = 'loadable'

libloadable_public_h = [
  'loadable-plugin.h',
]

libloadable_c = [
  'loadable-plugin.c',
]

libloadable_plugin_data = [
  'loadable.plugin',
]

libloadable_plugin_deps = [
  glib_dep,
  gobject_dep,
  libpeas_dep,
  libintrospection_dep,
]

libloadable_lib = shared_library(
  libloadable_name,
  libloadable_c,
  dependencies: libloadable_plugin_deps,
  install: false,
)

custom_target(
  'lib@0@-data'.format(libloadable_name),
  input: libloadable_plugin_data,
  output: libloadable_plugin_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
