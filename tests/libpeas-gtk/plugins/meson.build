libpeas_gtk_plugins_data = [
  'hidden.plugin',
  'invalid-custom-icon.plugin',
  'invalid-stock-icon.plugin',
  'valid-stock-icon.plugin',
]

custom_target(
  'libpeas-gtk-test-data',
  input: libpeas_gtk_plugins_data,
  output: libpeas_gtk_plugins_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)

subdir('builtin-configurable')
subdir('configurable')
subdir('valid-custom-icon')
