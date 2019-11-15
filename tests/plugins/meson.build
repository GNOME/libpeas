generic_name = 'generic'

generic_plugins_data = [
  'full-info.plugin',
  'min-info.plugin',
  'unavailable.plugin',
]

custom_target(
  '@0@-data'.format(generic_name),
  input: generic_plugins_data,
  output: generic_plugins_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)

subdir('builtin')
subdir('has-dep')
subdir('loadable')
subdir('self-dep')
