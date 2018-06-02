peas_demo_name = 'peas-demo'
peas_demo_string = '@0@-@1@'.format(peas_demo_name, api_version)

peas_demo_c = [
  'peas-demo-window.c',
  'peas-demo.c',
]

peas_demo_c_args = [
  '-DHAVE_CONFIG_H',
  '-DPEAS_BUILDDIR="@0@"'.format(builddir),
  '-DPEAS_PREFIX="@0@"'.format(prefix),
  '-DPEAS_LIBDIR="@0@"'.format(libdir),
]

executable(
  peas_demo_string,
  peas_demo_c,
  c_args: peas_demo_c_args,
  dependencies: [libpeas_gtk_dep],
)

subdir('plugins')