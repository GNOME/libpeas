python3_loader_name = 'pythonloader'

pygobject_version = pygobject_dep.version()
pygobject_version_arr = pygobject_version.split('.')
pygobject_version_major = pygobject_version_arr[0].to_int()
pygobject_version_minor = pygobject_version_arr[1].to_int()
pygobject_version_micro = pygobject_version_arr[2].to_int()

python3_loader_c = files(
  'peas-plugin-loader-python.c',
  'peas-python-internal.c',
)

python3_loader_res = gnome.compile_resources(
  'peas-python-resources',
  'peas-python.gresource.xml',
  export: false,
)

python3_loader_deps = [
  libpeas_dep,
  python3_dep,
  pygobject_dep,
]

python3_loader_c_args = [
  '-DPEAS_LOCALEDIR="@0@"'.format(localedir),
  '-DPYGOBJECT_MAJOR_VERSION=@0@'.format(pygobject_version_major),
  '-DPYGOBJECT_MINOR_VERSION=@0@'.format(pygobject_version_minor),
  '-DPYGOBJECT_MICRO_VERSION=@0@'.format(pygobject_version_micro),
]

python3_loader_sha = shared_module(
  python3_loader_name,
  python3_loader_c + python3_loader_res,
  include_directories: rootdir,
  dependencies: python3_loader_deps,
  c_args: python3_loader_c_args,
  install: true,
  install_dir: join_paths(
    pkglibdir,
    'loaders',
  ),
  name_suffix: module_suffix,
  gnu_symbol_visibility: 'hidden',
)
