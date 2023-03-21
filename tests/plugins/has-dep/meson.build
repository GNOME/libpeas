libhas_dep_name = 'has-dep'

libhas_dep_public_h = [
  'has-dep-plugin.h',
]

libhas_dep_c = [
  'has-dep-plugin.c',
]

libhas_dep_plugin_data = [
  'has-dep.plugin',
]

libhas_dep_plugin_deps = [
  glib_dep,
  gobject_dep,
  gmodule_dep,
  libpeas_dep,
  libintrospection_dep,
]

libhas_dep_lib = shared_library(
  libhas_dep_name,
  libhas_dep_c,
  dependencies: libhas_dep_plugin_deps,
  install: false,
)

custom_target(
  'lib@0@-data'.format(libhas_dep_name),
  input: libhas_dep_plugin_data,
  output: libhas_dep_plugin_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
