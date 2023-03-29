libpeas_srcdir = include_directories('.')

version_data = configuration_data()
version_data.set('PEAS_MAJOR_VERSION', version_major)
version_data.set('PEAS_MINOR_VERSION', version_minor)
version_data.set('PEAS_MICRO_VERSION', version_micro)
version_data.set('PEAS_VERSION', meson.project_version())

peas_version_h = configure_file(
  input: 'peas-version.h.in',
  output: 'peas-version.h',
  configuration: version_data,
  install_dir: pkgincludedir,
)

libpeas_public_h = files(
  'peas-engine.h',
  'peas-extension-base.h',
  'peas-extension-set.h',
  'peas-object-module.h',
  'peas-plugin-info.h',
  'peas-version-macros.h',
  'libpeas.h',
)

libpeas_sources = files(
  'peas-debug.c',
  'peas-dirs.c',
  'peas-engine.c',
  'peas-extension-base.c',
  'peas-extension-set.c',
  'peas-i18n.c',
  'peas-object-module.c',
  'peas-plugin-info.c',
  'peas-plugin-loader.c',
  'peas-plugin-loader-c.c',
  'peas-utils.c',
)

if build_machine.system() == 'darwin'
  libpeas_sources += [
    'peas-utils-osx.m',
    'peas-utils-osx.h',
  ]
endif


libpeas_deps = [
  glib_dep,
  gobject_dep,
  gmodule_dep,
  gio_dep,
]

libpeas_c_args = [
  '-DG_LOG_DOMAIN="libpeas"',
  '-DDATADIR="@0@"'.format(datadir),
  '-DLIBDIR="@0@"'.format(libdir),
]

libpeas_link_args = [
]

if build_machine.system() == 'darwin'
  libpeas_link_args += [
    '-Wl,-framework', '-Wl,Foundation',
    '-Wl,-framework', '-Wl,AppKit',
  ]
endif

libpeas_marshal = gnome.genmarshal(
  'peas-marshal',
  sources: 'peas-marshal.list',
  prefix: 'peas_cclosure_marshal',
  install_header: false,
  valist_marshallers: true,
)

libpeas_marshals_h = libpeas_marshal.get(1)

install_headers(libpeas_public_h, install_dir: pkgincludedir)

libpeas_sha = library(
  package_string.strip('lib'),
  libpeas_sources + libpeas_marshal,
  version: lib_version,
  darwin_versions: lib_version_osx,
  include_directories: [rootdir, libpeas_srcdir],
  dependencies: libpeas_deps,
  c_args: libpeas_c_args + hidden_visibility_args,
  objc_args: libpeas_c_args,
  link_args: libpeas_link_args,
  install: true,
)

libpeas_dep_sources = [
  libpeas_marshals_h,
]

if generate_gir == true
  libpeas_gir_includes = [
    'GObject-2.0',
    'GModule-2.0',
    'Gio-2.0',
  ]

  libpeas_gir = gnome.generate_gir(
    libpeas_sha,
    sources: libpeas_sources + libpeas_public_h,
    nsversion: api_version,
    namespace: 'Peas',
    symbol_prefix: 'peas',
    identifier_prefix: 'Peas',
    header: 'libpeas.h',
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

  if generate_vapi == true
    vapi_dependencies = [
      'gmodule-2.0',
      'gio-2.0',
    ]
    gnome.generate_vapi(
      package_string,
      install: true,
      packages: vapi_dependencies,
      sources: libpeas_gir[0]
    )
  endif

  libpeas_dep_sources += [
    libpeas_gir,
  ]
endif

libpeas_dep = declare_dependency(
  link_with: libpeas_sha,
  include_directories: [rootdir, libpeas_srcdir],
  dependencies: libpeas_deps,
  sources: libpeas_dep_sources,
)

libpeas_pc_reqs = [
  'glib-2.0 @0@'.format(glib_req),
  'gobject-2.0 @0@'.format(glib_req),
  'gmodule-2.0 @0@'.format(glib_req),
  'gio-2.0 @0@'.format(glib_req),
]

libpeas_pc = pkg.generate(
  libpeas_sha,
  name: package_string,
  description: 'GObject plugins library',
  subdirs: package_string,
  install_dir: pkgconfigdir,
  requires: libpeas_pc_reqs,
)
