glib_prefix = glib_dep.get_pkgconfig_variable('prefix')
glib_docpath = join_paths(glib_prefix, 'share', 'gtk-doc', 'html')

gtk_prefix = gtk_dep.get_pkgconfig_variable('prefix')
gtk_docpath = join_paths(gtk_prefix, 'share', 'gtk-doc', 'html')

libpeas_reference_ignored_h = [
  'gconstructor.h',
  'peas-debug.h',
  'peas-dirs.h',
  'peas-engine-priv.h',
  'peas-introspection.h',
  'peas-marshal.h',
  'peas-plugin-info-priv.h',
  'peas-plugin-loader.h',
  'peas-plugin-loader-c.h',
  'peas-utils.h',
]

libpeas_reference_html_images = [
]

libpeas_reference_srcdirs = [
  libpeas_srcdir,
]

if build_gtk_widgetry == true
  libpeas_reference_ignored_h += [
    'peas-gtk-disable-plugins-dialog.h',
    'peas-gtk-plugin-manager-store.h',
  ]

  libpeas_reference_html_images += [
    join_paths('images', 'peas-gtk-plugin-manager.png'),
  ]

  libpeas_reference_srcdirs += [
    libpeas_gtk_srcdir,
  ]
endif

version_xml = configuration_data()
version_xml.set('PEAS_VERSION', version)

configure_file(
  input: 'version.xml.in',
  output: 'version.xml',
  configuration: version_xml,
)

libpeas_reference_deps = [
  libpeas_dep,
]

libpeas_reference_scan_args = [
  '--rebuild-types',
  '--deprecated-guards="PEAS_DISABLE_DEPRECATED"',
  '--ignore-headers=' + ' '.join(libpeas_reference_ignored_h),
]

libpeas_reference_fixxref_args = [
  '--extra-dir=@0@'.format(join_paths(glib_docpath, 'glib')),
  '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gobject')),
  '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gmodule')),
  '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gio')),
]

libpeas_reference_mkdb_args = [
  '--sgml-mode',
  '--output-format=xml',
]

if build_gtk_widgetry == true
  libpeas_reference_deps += [
    libpeas_gtk_dep,
  ]

  libpeas_reference_fixxref_args += [
    '--extra-dir=@0@'.format(join_paths(gtk_docpath, 'gtk')),
  ]
endif

gnome.gtkdoc(
  package_name,
  main_sgml: 'libpeas-docs.sgml',
  gobject_typesfile: 'libpeas.types',
  src_dir: libpeas_reference_srcdirs,
  html_assets: libpeas_reference_html_images,
  dependencies: libpeas_reference_deps,
  scan_args: libpeas_reference_scan_args,
  fixxref_args: libpeas_reference_fixxref_args,
  mkdb_args: libpeas_reference_mkdb_args,
  install: true,
  install_dir: join_paths(
    gtkdocdir,
    'html',
    package_name,
  )
)
