libpeas_plugins_data = [
  'extension-c-nonexistent.plugin',
  'extension-gjs-nonexistent.plugin',
  'extension-lua51-nonexistent.plugin',
  'extension-python-nonexistent.plugin',
  'info-missing-module.plugin',
  'info-missing-name.plugin',
  'invalid.plugin',
  'nonexistent-dep.plugin',
  'not-loadable.plugin',
  'os-dependant-help.plugin',
  'two-deps.plugin',
  'unkown-loader.plugin',
]

custom_target(
  'libpeas-test-data',
  input: libpeas_plugins_data,
  output: libpeas_plugins_data,
  command: [copyfile_script, '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)

subdir('embedded')
subdir('extension-c')

if build_gjs_loader == true
  subdir('extension-gjs')
endif

if build_lua51_loader == true
  subdir('extension-lua')
endif

if build_python3_loader == true
  subdir('extension-python')
endif
