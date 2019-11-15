luahello_name = 'luahello'

luahello_plugin_data = [
  'luahello.lua',
  'luahello.plugin',
]

custom_target(
  '@0@-data'.format(luahello_name),
  input: luahello_plugin_data,
  output: luahello_plugin_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
  install_dir: join_paths(peas_demo_plugins_dir, 'luahello'),
  install: true,
)
