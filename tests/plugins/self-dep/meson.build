libself_dep_name = 'self-dep'

libself_dep_public_h = [
  'self-dep-plugin.h',
]

libself_dep_c = [
  'self-dep-plugin.c',
]

libself_dep_plugin_data = [
  'self-dep.plugin',
]

libself_dep_plugin_deps = [
  glib_dep,
  gobject_dep,
  libpeas_dep,
  libintrospection_dep,
]

libself_dep_lib = shared_library(
  libself_dep_name,
  libself_dep_c,
  dependencies: libself_dep_plugin_deps,
  install: false,
)

custom_target(
  'lib@0@-data'.format(libself_dep_name),
  input: libself_dep_plugin_data,
  output: libself_dep_plugin_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
