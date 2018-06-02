libembedded_name = 'embedded'

libembedded_public_h = [
  'embedded-plugin.h',
]

libembedded_c = [
  'embedded-plugin.c',
]

libembedded_plugin_data = [
  'embedded.gresource.xml',
  'embedded.plugin',
  'embedded-invalid-loader.plugin',
  'embedded-missing-symbol.plugin',
]

libembedded_res = gnome.compile_resources(
  'embedded-resources',
  'embedded.gresource.xml',
  export: true,
  extra_args: [
    '--manual-register',
  ],
)

libembedded_deps = [
  glib_dep,
  libpeas_dep,
]

libembedded_lib = shared_library(
  libembedded_name,
  libembedded_c + libembedded_res,
  include_directories: rootdir,
  dependencies: libembedded_deps,
  install: false,
)

libembedded_dep = declare_dependency(
  link_with: libembedded_lib,
  include_directories: include_directories('.'),
  dependencies: libembedded_deps,
)

custom_target(
  '@0@-data'.format(libembedded_name),
  input: libembedded_plugin_data,
  output: libembedded_plugin_data,
  command: ['cp', '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
