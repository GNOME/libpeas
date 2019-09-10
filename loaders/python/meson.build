python2_loader_name = 'pythonloader'

pygobject_version = pygobject_dep.version()
pygobject_version_arr = pygobject_version.split('.')
pygobject_version_major = pygobject_version_arr[0].to_int()
pygobject_version_minor = pygobject_version_arr[1].to_int()
pygobject_version_micro = pygobject_version_arr[2].to_int()

python2_loader_c = files(
  'peas-plugin-loader-python.c',
  'peas-python-internal.c',
)

python2_loader_res = gnome.compile_resources(
  'peas-python-resources',
  'peas-python.gresource.xml',
  export: true,
)

python2_loader_deps = [
  libpeas_dep,
  python2_dep,
  pygobject_dep,
]

python2_loader_c_args = [
  '-DHAVE_CONFIG_H',
  '-DPEAS_LOCALEDIR="@0@"'.format(localedir),
  '-DPYGOBJECT_MAJOR_VERSION=@0@'.format(pygobject_version_major),
  '-DPYGOBJECT_MINOR_VERSION=@0@'.format(pygobject_version_minor),
  '-DPYGOBJECT_MICRO_VERSION=@0@'.format(pygobject_version_micro),
]

python2_loader_sha = shared_module(
  python2_loader_name,
  python2_loader_c + python2_loader_res,
  include_directories: rootdir,
  dependencies: python2_loader_deps,
  c_args: python2_loader_c_args,
  install: true,
  install_dir: join_paths(
    pkglibdir,
    'loaders',
  ),
  name_suffix: module_suffix
)
