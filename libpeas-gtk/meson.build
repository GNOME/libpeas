libpeas_gtk_srcdir = include_directories('.')

libpeas_gtk_public_h = files(
  'peas-gtk-autocleanups.h',
  'peas-gtk-configurable.h',
  'peas-gtk-plugin-manager.h',
  'peas-gtk-plugin-manager-view.h',
  'peas-gtk.h',
)

libpeas_gtk_c = files(
  'peas-gtk-configurable.c',
  'peas-gtk-disable-plugins-dialog.c',
  'peas-gtk-plugin-manager.c',
  'peas-gtk-plugin-manager-store.c',
  'peas-gtk-plugin-manager-view.c',
)

libpeas_gtk_deps = [
  glib_dep,
  gobject_dep,
  introspection_dep,
  gtk_dep,
  libpeas_dep,
]

libpeas_gtk_c_args = [
  '-DHAVE_CONFIG_H',
  '-DG_LOG_DOMAIN="libpeas-gtk"',
]

libpeas_gtk_c_args += hidden_visibility_args

libpeas_gtk_link_args = [
]

install_headers(
  libpeas_gtk_public_h,
  install_dir: join_paths(
    pkgincludedir,
    package_gtk_name,
  )
)

libpeas_gtk_sha = library(
  package_gtk_string.strip('lib'),
  libpeas_gtk_c,
  version: lib_version,
  darwin_versions: lib_version_osx,
  include_directories: rootdir,
  dependencies: libpeas_gtk_deps,
  c_args: libpeas_gtk_c_args,
  link_args: libpeas_gtk_link_args,
  install: true,
)

libpeas_gtk_dep_sources = []

if generate_gir == true
  libpeas_gtk_gir_includes = [
    'GObject-2.0',
    'Gtk-3.0',
    libpeas_gir.get(0)
  ]

  libpeas_gtk_gir = gnome.generate_gir(
    libpeas_gtk_sha,
    sources: libpeas_gtk_c + libpeas_gtk_public_h,
    nsversion: api_version,
    namespace: 'PeasGtk',
    symbol_prefix: 'peas_gtk',
    identifier_prefix: 'PeasGtk',
    header: 'libpeas-gtk/peas-gtk.h',
    export_packages: [package_gtk_string],
    includes: libpeas_gtk_gir_includes,
    link_with: [libpeas_sha, libpeas_gtk_sha],
    install: true,
    install_dir_gir: girdir,
    install_dir_typelib: typelibdir,
    extra_args: [
      '--warn-all',
    ],
  )

  libpeas_gtk_dep_sources += [
    libpeas_gtk_gir,
  ]
endif

libpeas_gtk_dep = declare_dependency(
  link_with: libpeas_gtk_sha,
  dependencies: libpeas_gtk_deps,
  include_directories: rootdir,
  sources: libpeas_gtk_dep_sources,
)

libpeas_gtk_pc_reqs = [
  glib_dep,
  gtk_dep,
  package_string + ' >= @0@'.format(version)
]

libpeas_gtk_pc = pkg.generate(
  libpeas_gtk_sha,
  name: package_gtk_string,
  description: 'GObject plugins library widgetery',
  requires: libpeas_gtk_pc_reqs,
  subdirs: package_string,
  install_dir: pkgconfigdir,
)
