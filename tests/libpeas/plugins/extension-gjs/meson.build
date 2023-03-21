extension_gjs_name = 'extension-gjs'

extension_gjs_plugin_data = [
  'extension-gjs.gschema.xml',
  'extension-gjs.js',
  'extension-gjs.plugin',
]

custom_target(
  '@0@-data'.format(extension_gjs_name),
  input: extension_gjs_plugin_data,
  output: extension_gjs_plugin_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
