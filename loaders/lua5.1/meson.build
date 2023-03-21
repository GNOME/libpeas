lua51_loader_name = 'lua51loader'

lua51_lgi_version = lua51_lgi_dep.version()
lua51_lgi_version_arr = lua51_lgi_version.split('.')
lua51_lgi_version_major = lua51_lgi_version_arr[0].to_int()
lua51_lgi_version_minor = lua51_lgi_version_arr[1].to_int()
lua51_lgi_version_micro = lua51_lgi_version_arr[2].to_int()

lua51_loader_c = files(
  'peas-lua-internal.c',
  'peas-lua-utils.c',
  'peas-plugin-loader-lua.c',
)

subdir('resources') # Declares 'lua51_loader_res'

lua51_loader_deps = [
  libpeas_dep,
  lua51_dep,
  lua51_lgi_dep,
  luajit_dep,
]

lua51_loader_c_args = [
  '-DLGI_MAJOR_VERSION=@0@'.format(lua51_lgi_version_major),
  '-DLGI_MINOR_VERSION=@0@'.format(lua51_lgi_version_minor),
  '-DLGI_MICRO_VERSION=@0@'.format(lua51_lgi_version_micro),
]

lua51_loader_sha = shared_module(
  lua51_loader_name,
  lua51_loader_c + lua51_loader_res,
  include_directories: rootdir,
  dependencies: lua51_loader_deps,
  c_args: lua51_loader_c_args,
  install: true,
  install_dir: join_paths(
    pkglibdir,
    'loaders',
  ),
  name_suffix: module_suffix,
  gnu_symbol_visibility: 'hidden',
)
