valid_custom_icon_name = 'valid-custom-icon'

valid_custom_icon_plugin_data = [
  'exists.png',
  'valid-custom-icon.plugin',
]

custom_target(
  '@0@-data'.format(valid_custom_icon_name),
  input: valid_custom_icon_plugin_data,
  output: valid_custom_icon_plugin_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
