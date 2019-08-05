libpeas_srcdir = include_directories('.')

libpeas_public_h = files(
  'peas-activatable.h',
  'peas-autocleanups.h',
  'peas-engine.h',
  'peas-extension.h',
  'peas-extension-base.h',
  'peas-extension-set.h',
  'peas-object-module.h',
  'peas-plugin-info.h',
  'peas.h',
)

libpeas_sources = files(
  'peas-activatable.c',
  'peas-debug.c',
  'peas-dirs.c',
  'peas-engine.c',
  'peas-extension.c',
  'peas-extension-base.c',
  'peas-extension-set.c',
  'peas-i18n.c',
  'peas-introspection.c',
  'peas-object-module.c',
  'peas-plugin-info.c',
  'peas-plugin-loader.c',
  'peas-plugin-loader-c.c',
  'peas-utils.c',
)

libpeas_deps = [
  glib_dep,
  gobject_dep,
  gmodule_dep,
  gio_dep,
  introspection_dep,
]

libpeas_c_args = [
  '-DHAVE_CONFIG_H',
  '-DG_LOG_DOMAIN="libpeas"',
  '-DDATADIR="@0@"'.format(datadir),
  '-DLIBDIR="@0@"'.format(libdir),
]

libpeas_link_args = [
]

libpeas_marshal = gnome.genmarshal(
  'peas-marshal',
  sources: 'peas-marshal.list',
  prefix: 'peas_cclosure_marshal',
  install_header: false,
)

libpeas_marshals_h = libpeas_marshal.get(1)

libpeas_sources += [
  libpeas_marshal,
]

install_headers(
  libpeas_public_h,
  install_dir: join_paths(
    pkgincludedir,
    package_name,
  ),
)

libpeas_sha = library(
  package_string.strip('lib'),
  libpeas_sources,
  version: lib_version,
  include_directories: rootdir,
  dependencies: libpeas_deps,
  c_args: libpeas_c_args,
  link_args: libpeas_link_args,
  install: true,
  install_dir: libdir,
)

libpeas_dep_sources = [
  libpeas_marshals_h,
]

if generate_gir == true
  libpeas_gir_includes = [
    'GObject-2.0',
    'GModule-2.0',
    'Gio-2.0',
    'GIRepository-2.0'
  ]

  libpeas_gir = gnome.generate_gir(
    libpeas_sha,
    sources: libpeas_sources + libpeas_public_h,
    nsversion: api_version,
    namespace: 'Peas',
    symbol_prefix: 'peas',
    identifier_prefix: 'Peas',
    header: 'libpeas/peas.h',
    export_packages: [package_string],
    includes: libpeas_gir_includes,
    link_with: libpeas_sha,
    install: true,
    install_dir_gir: girdir,
    install_dir_typelib: typelibdir,
    extra_args: [
      '--warn-all',
    ],
  )

  libpeas_dep_sources += [
    libpeas_gir,
  ]
endif

libpeas_dep = declare_dependency(
  link_with: libpeas_sha,
  include_directories: rootdir,
  dependencies: libpeas_deps,
  sources: libpeas_dep_sources,
)

libpeas_pc = pkg.generate(
  libpeas_sha,
  name: package_string,
  description: 'GObject plugins library',
  subdirs: package_string,
  install_dir: pkgconfigdir,
)