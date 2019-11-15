libextension_c_name = 'extension-c'
libextension_c_missing_name = 'extension-c-missing-symbol'

libextension_c_public_h = [
  'extension-c-abstract.h',
  'extension-c-plugin.h',
]

libextension_c_c = [
  'extension-c-abstract.c',
  'extension-c-plugin.c',
]

libextension_c_plugin_data = [
  'extension-c.gschema.xml',
  'extension-c.plugin',
]

libextension_c_deps = [
  glib_dep,
  gmodule_dep,
  libpeas_dep,
  libintrospection_dep,
]

libextension_c_lib = shared_library(
  libextension_c_name,
  libextension_c_c,
  include_directories: rootdir,
  dependencies: libextension_c_deps,
  install: false,
)

custom_target(
  'lib@0@-data'.format(libextension_c_name),
  input: libextension_c_plugin_data,
  output: libextension_c_plugin_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)

libextension_c_missing_c = [
  'extension-c-missing-symbol-plugin.c',
]

libextension_c_missing_plugin_data = [
  'extension-c-missing-symbol.plugin',
]

libextension_c_missing_lib = shared_library(
  libextension_c_missing_name,
  libextension_c_missing_c,
  include_directories: rootdir,
  dependencies: libextension_c_deps,
  install: false,
)

custom_target(
  'lib@0@-data'.format(libextension_c_missing_name),
  input: libextension_c_missing_plugin_data,
  output: libextension_c_missing_plugin_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
