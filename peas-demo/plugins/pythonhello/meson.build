pythonhello_name = 'pythonhello'

pythonhello_plugin_data = [
  'pythonhello.py',
  'pythonhello.plugin',
]

custom_target(
  '@0@-data'.format(pythonhello_name),
  input: pythonhello_plugin_data,
  output: pythonhello_plugin_data,
  command: ['cp', '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)
