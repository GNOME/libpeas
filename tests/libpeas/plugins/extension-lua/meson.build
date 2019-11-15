extension_lua_name = 'extension-lua'

extension_lua_plugin_data = [
  'extension-lua51.gschema.xml',
  'extension-lua51.lua',
  'extension-lua51.plugin',
]

custom_target(
  '@0@-data'.format(extension_lua_name),
  input: extension_lua_plugin_data,
  output: extension_lua_plugin_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
