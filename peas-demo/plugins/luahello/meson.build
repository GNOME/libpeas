luahello_name = 'luahello'

luahello_plugin_data = [
  'luahello.lua',
  'luahello.plugin',
]

custom_target(
  '@0@-data'.format(luahello_name),
  input: luahello_plugin_data,
  output: luahello_plugin_data,
  command: ['cp', '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
