expand_content_md_files = [
  'migrating-1to2.md',
]

toml_data = configuration_data()
toml_data.set('VERSION', meson.project_version())

libpeas_toml = configure_file(
  input: 'libpeas.toml.in',
  output: 'libpeas.toml',
  configuration: toml_data
)

gidocgen = find_program('gi-docgen')

docs_dir = get_option('datadir') / 'doc'

custom_target('libpeas-doc',
  input: [libpeas_toml, libpeas_gir[0]],
  output: 'libpeas-@0@'.format(api_version),
  command: [
    gidocgen,
    'generate',
    '--quiet',
    '--add-include-path=@0@'.format(meson.current_build_dir() / '../../libpeas'),
    '--config=@INPUT0@',
    '--output-dir=@OUTPUT@',
    '--no-namespace-dir',
    '--content-dir=@0@'.format(meson.current_source_dir()),
    '@INPUT1@',
  ],
  depend_files: [ expand_content_md_files ],
  build_by_default: true,
  install: true,
  install_dir: docs_dir,
)
