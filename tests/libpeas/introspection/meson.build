libintrospection_name = 'introspection'
libintrospection_string = '@0@-@1@'.format(libintrospection_name, api_version)

libintrospection_public_h = [
  'introspection-abstract.h',
  'introspection-activatable.h',
  'introspection-base.h',
  'introspection-callable.h',
  'introspection-has-prerequisite.h',
  'introspection-prerequisite.h',
  'introspection-unimplemented.h',
]

libintrospection_c = [
  'introspection-abstract.c',
  'introspection-activatable.c',
  'introspection-base.c',
  'introspection-callable.c',
  'introspection-has-prerequisite.c',
  'introspection-prerequisite.c',
  'introspection-unimplemented.c',
]

libintrospection_deps = [
  glib_dep,
  gobject_dep,
  libpeas_dep,
]

libintrospection_c_args = [
]

libintrospection_lib = library(
  libintrospection_string,
  libintrospection_c,
  include_directories: rootdir,
  dependencies: libintrospection_deps,
  c_args: libintrospection_c_args,
  install: false,
)

libintrospection_dep = declare_dependency(
  link_with: libintrospection_lib,
  include_directories: include_directories('.'),
  dependencies: libintrospection_deps,
)

libintrospection_gir = gnome.generate_gir(
  libintrospection_lib,
  sources: libintrospection_c + libintrospection_public_h,
  nsversion: api_version,
  namespace: 'Introspection',
  symbol_prefix: 'introspection',
  identifier_prefix: 'Introspection',
  dependencies: [libintrospection_dep],
  export_packages: [libintrospection_string],
  link_with: libintrospection_lib,
  includes: ['GObject-2.0', libpeas_gir.get(0)],
  install: false,
  extra_args: [
    '--warn-all',
  ],
)

libintrospection_gir_dep = declare_dependency(
  sources: libintrospection_gir
)
