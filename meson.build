project(
  'libpeas', 'c',
  version: '2.2.1',
  license: 'LGPLv2.1+',
  meson_version: '>= 1.0',
  default_options: [
    'buildtype=debugoptimized',
    'cpp_std=c++17',
    'warning_level=2',
  ]
)

gnome = import('gnome')
i18n = import('i18n')
pkg = import('pkgconfig')
pymod = import('python')

# Versioning
version = meson.project_version()
version_arr = version.split('.')
version_major = version_arr[0].to_int()
version_minor = version_arr[1].to_int()
version_micro = version_arr[2].to_int()

if version_minor == 99
  api_version = '@0@'.format(version_major+1)
else
  api_version = '@0@'.format(version_major)
endif

if version_minor.is_odd()
  interface_age = 0
else
  interface_age = version_micro
endif

# Compatibility with libtool versioning:
#  current = minor * 100 + micro - interface
#  revision = interface
lib_version_soversion = 0
lib_version_current = 0
if version_minor != 99
  lib_version_current = version_minor * 100 + version_micro - interface_age
endif
lib_version_revision = interface_age
lib_version = '@0@.@1@.@2@'.format(
  lib_version_soversion,
  lib_version_current,
  lib_version_revision
)

osx_current = lib_version_current + 1
lib_version_osx = [osx_current, '@0@.@1@'.format(osx_current, interface_age)]

package_name = meson.project_name()
package_string = '@0@-@1@'.format(package_name, api_version)
package_long_string = '@0@-@1@'.format(package_name, version)
package_gtk_name = '@0@-gtk'.format(meson.project_name())
package_gtk_string = '@0@-@1@'.format(package_gtk_name, api_version)

# Paths
rootdir = include_directories('.')
srcdir = meson.current_source_dir()
builddir = meson.current_build_dir()

prefix = get_option('prefix')

includedir = join_paths(prefix, get_option('includedir'))
libdir = join_paths(prefix, get_option('libdir'))
datadir = join_paths(prefix, get_option('datadir'))
localedir = join_paths(prefix, get_option('localedir'))

pkgincludedir = join_paths(includedir, package_string)
pkglibdir = join_paths(libdir, package_string)
pkgdatadir = join_paths(datadir, package_string)

girdir = join_paths(datadir, 'gir-1.0')
iconsdir = join_paths(datadir, 'icons')
pkgconfigdir = join_paths(libdir, 'pkgconfig')
typelibdir = join_paths(libdir, 'girepository-1.0')

# Configurations
config_h = configuration_data()
config_h.set_quoted('PACKAGE', package_name)
config_h.set_quoted('PACKAGE_BUGREPORT', 'https://gitlab.gnome.org/GNOME/libpeas/-/issues/')
config_h.set_quoted('PACKAGE_NAME', package_name)
config_h.set_quoted('PACKAGE_STRING', package_long_string)
config_h.set_quoted('PACKAGE_URL', 'https://wiki.gnome.org/Projects/Libpeas')
config_h.set_quoted('PACKAGE_URL', 'https://wiki.gnome.org/Projects/Libpeas')
config_h.set_quoted('PACKAGE_VERSION', version)
config_h.set_quoted('VERSION', version)
config_h.set_quoted('API_VERSION_S', '@0@'.format(api_version))

config_h.set_quoted('GETTEXT_PACKAGE', package_string)

# Dependencies
cc = meson.get_compiler('c')

glib_req_version = '2.86'

glib_req = '>= @0@'.format(glib_req_version)
gtk_doc_req = '>= 1.11'
python3_req = '>= 3.2.0'
pygobject_req = '>= 3.2.0'
lua_req = '>= 5.1.0'
lua_lgi_req = '>= 0.9.0'
luajit_req = '>= 2.0'
gjs_req = '>= 1.77.1'

glib_dep = dependency('glib-2.0', version: glib_req)
gobject_dep = dependency('gobject-2.0', version: glib_req)
gmodule_dep = dependency('gmodule-2.0', version: glib_req)
gio_dep = dependency('gio-2.0', version: glib_req)
introspection_dep = dependency('girepository-2.0', version: glib_req)

gi_docgen_dep = dependency('gi-docgen', version: '>= 2021.7',
                           fallback: ['gi-docgen', 'dummy_dep'],
                           native: true,
                           required: get_option('gtk_doc'))
gjs_dep = dependency('gjs-1.0', version: gjs_req, required: get_option('gjs'))
if gjs_dep.found()
  mozjs_dep = dependency(gjs_dep.get_variable(pkgconfig: 'mozjs_dep_name', default_value: 'mozjs-128'), required: true)
else
  mozjs_dep = disabler()
endif

# Check if user specified a Python installation path, if so, use it
python_path = get_option('python3_path')
python3_dep = disabler()

if python_path == ''
  # From python 3.8 we neeed python3-embed
  python3_dep = dependency('python3-embed', required: false)
  if not python3_dep.found()
    python3_dep = dependency('python3', version: python3_req, required: false)
  endif
endif

if not python3_dep.found() or python_path != ''
  python_inst = pymod.find_installation(python_path, required: false)
  if python_inst.found()
    python3_dep = python_inst.dependency(embed: true, version: python3_req, required: false)
  endif
endif

pygobject_dep = dependency('pygobject-3.0', version: pygobject_req, required: false)
lua51_dep = dependency('lua51', version: lua_req, required: false)
if not lua51_dep.found()
  lua51_dep = dependency('lua-5.1', version: lua_req, required: false)
endif
luajit_dep = dependency('luajit', version: luajit_req, required: false)
lua_lgi_found = false
lua_lgi_ver = 'not found'
lua51_prg = find_program('lua5.1', required: false)
if not lua51_prg.found()
  lua51_prg = find_program('lua51', required: false)
endif
luajit_prg = find_program('luajit', required: false)
xmllint_prg = find_program('xmllint', required: false)
generate_vapi = get_option('vapi')
vapigen_prg = find_program('vapigen', required: generate_vapi)

if cc.get_id() == 'msvc'
  if luajit_prg.found()
    # luajit has lua51.lib as its import library
    lua_names = ['lua51']
  else
    lua_names = ['lua54', 'lua53', 'lua52', 'lua51']
  endif

  lua_headers = ['lua.h', 'lualib.h', 'lauxlib.h']
  # On Windows, the lua program may be named as lua.exe
  foreach lua: lua_names + ['lua']
    if not lua51_prg.found() and not luajit_prg.found()
      lua51_prg = find_program(lua, required: false)
    endif
    if lua51_prg.found()
      if lua != 'lua'
        lua51_dep = cc.find_library(lua, has_headers: lua_headers, required: false)
      endif
    endif
  endforeach

  if not lua51_dep.found() and not luajit_dep.found()
    foreach lualib: lua_names
      if luajit_prg.found()
        if not luajit_dep.found()
          luajit_dep = cc.find_library(lualib, has_headers: lua_headers, required: false)
        endif
      endif

      if lua51_prg.found()
        if not lua51_dep.found()
          lua51_dep = cc.find_library(lualib, has_headers: lua_headers, required: false)
        endif
      endif
    endforeach
  endif
endif

lua_found = false

if (luajit_dep.found() and luajit_prg.found()) or (lua51_dep.found() and lua51_prg.found())
  lua_found = true
endif

if lua_found
    if luajit_prg.found()
        lua_prg = luajit_prg
    else
        lua_prg = lua51_prg
    endif
    lua_lgi_check = run_command(lua_prg, ['-e', 'print(require("lgi")._VERSION)'], check: false)
    if lua_lgi_check.returncode() == 0
        lua_lgi_ver = lua_lgi_check.stdout().strip()
        if lua_lgi_ver.version_compare(lua_lgi_req)
            lua51_lgi_dep = declare_dependency(version: lua_lgi_ver)
            lua_lgi_found = true
        endif
    endif
    message('lua-lgi version: ' + lua_lgi_ver)
endif

glib_version_arr = glib_req_version.split('.')
glib_major_version = glib_version_arr[0]
glib_minor_version = glib_version_arr[1]
if glib_minor_version.to_int() % 2 == 1
  glib_minor_version = '@0@'.format(glib_minor_version.to_int() + 1)
endif

project_c_args = [
  '-DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_@0@_@1@'.format(glib_major_version, glib_minor_version),
  '-DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_@0@_@1@'.format(glib_major_version, glib_minor_version),
  '-DLIBPEAS_COMPILATION',
]
add_project_arguments(project_c_args, language: 'c')

global_c_args = []
global_link_args = []

test_c_args = []
if cc.get_id() == 'msvc'
  # Look for msvc_recommended_pragmas.h first in GLib's includedir if
  # GLib is found, otherwise look for it in GLib's subproject directory
  # then as a standalone header (i.e. somewhere in %INCLUDE%)
  # Force-include it in our builds if it is found

  msvc_force_include_h = 'msvc_recommended_pragmas.h'
  msvc_force_include_h_cflags = []
  if glib_dep.type_name() == 'pkgconfig'
    glib_include_dir = glib_dep.get_variable(pkgconfig : 'includedir') / 'glib-2.0'
    msvc_force_include_h_cflags += \
      '-FI@0@/@1@'.format(glib_include_dir, msvc_force_include_h)
  elif glib_dep.type_name() == 'internal'
    glib_subproject_dir = meson.source_root() / 'subprojects' / 'glib'

    # Drop the second one once we require glib-2.77.x or later
    msvc_force_include_h_cflags += [
      '-FI@0@/@1@'.format(glib_subproject_dir / 'glib', msvc_force_include_h),
      '-FI@0@/@1@'.format(glib_subproject_dir, msvc_force_include_h)
    ]
  endif
  msvc_force_include_h_cflags += '-FI@0@'.format(msvc_force_include_h)
  test_c_args += msvc_force_include_h_cflags

  c_fi_carg_found = false
  foreach fi_carg : msvc_force_include_h_cflags
    if not c_fi_carg_found
      if cc.has_multi_arguments(fi_carg)
        global_c_args += fi_carg
        c_fi_carg_found = true
      endif
    endif
  endforeach
else
  test_c_args += [
    '-Wcast-align',
    '-Werror=address',
    '-Werror=array-bounds',
    '-Werror=empty-body',
    '-Werror=implicit',
    '-Werror=implicit-function-declaration',
    '-Werror=incompatible-pointer-types',
    '-Werror=init-self',
    '-Werror=int-conversion',
    '-Werror=int-to-pointer-cast',
    '-Werror=main',
    '-Werror=misleading-indentation',
    '-Werror=missing-braces',
    '-Werror=missing-include-dirs',
    '-Werror=nonnull',
    '-Werror=overflow',
    '-Werror=parenthesis',
    '-Werror=pointer-arith',
    '-Werror=pointer-to-int-cast',
    '-Werror=return-type',
    '-Werror=sequence-point',
    '-Werror=shadow',
    '-Werror=trigraphs',
    '-Werror=undef',
    '-Werror=write-strings',
    '-Wformat-nonliteral',
    ['-Werror=format-security', '-Werror=format=2'],
    '-Wignored-qualifiers',
    '-Wimplicit-function-declaration',
    '-Wlogical-op',
    '-Wmissing-declarations',
    '-Wmissing-format-attribute',
    '-Wmissing-include-dirs',
    '-Wmissing-noreturn',
    '-Wnested-externs',
    '-Wno-cast-function-type',
    '-Wno-dangling-pointer',
    '-Wno-missing-field-initializers',
    '-Wno-sign-compare',
    '-Wno-unused-parameter',
    '-Wold-style-definition',
    '-Wpointer-arith',
    '-Wredundant-decls',
    '-Wstrict-prototypes',
    '-Wswitch-default',
    '-Wswitch-enum',
    '-Wundef',
    '-Wuninitialized',
    '-Wunused',
    '-fno-strict-aliasing',
  ]
  foreach arg: test_c_args
    if cc.has_multi_arguments(arg)
      global_c_args += arg
    endif
  endforeach
endif

test_link_args = []

if cc.get_id() != 'msvc'
  test_link_args += [
    '-Wl,-z,relro',
    '-Wl,-z,now',
  ]
endif

if not get_option('buildtype').startswith('debug')
  global_c_args += ['-DG_DISABLE_CAST_CHECKS', '-DG_DISABLE_ASSERT']
  if cc.get_id() != 'msvc'
    test_link_args += [
      '-Wl,-Bsymbolic',
      '-fno-plt',
    ]
  endif
endif
foreach link_arg: test_link_args
  if cc.has_link_argument(link_arg)
    global_link_args += link_arg
  endif
endforeach

add_project_arguments(global_c_args, language: 'c')
add_project_link_arguments(global_link_args, language: 'c')

module_suffix = []
if build_machine.system() == 'darwin'
  config_h.set('OS_OSX', 1)
  add_languages('objc')
  module_suffix = 'so'
endif

# Detect and set symbol visibility
hidden_visibility_args = []
if get_option('default_library') != 'static'
  if host_machine.system() == 'windows'
    config_h.set('DLL_EXPORT', true)
    if cc.get_id() == 'msvc'
      config_h.set('_PEAS_EXTERN', '__declspec(dllexport) extern')
    elif cc.has_argument('-fvisibility=hidden')
      config_h.set('_PEAS_EXTERN', '__attribute__((visibility("default"))) __declspec(dllexport) extern')
      hidden_visibility_args = ['-fvisibility=hidden']
    endif
  elif cc.has_argument('-fvisibility=hidden')
    config_h.set('_PEAS_EXTERN', '__attribute__((visibility("default"))) extern')
    hidden_visibility_args = ['-fvisibility=hidden']
  endif
endif

configure_file(
  output: 'config.h',
  configuration: config_h
)

# Options
build_gtk_doc = get_option('gtk_doc') and gi_docgen_dep.found()

generate_gir = get_option('introspection')
if generate_gir and not introspection_dep.found()
  generate_gir = false
endif

if not generate_gir
  generate_vapi = false
endif

build_gjs_loader = get_option('gjs')

build_lua51_loader = get_option('lua51')
lua51_found = (luajit_dep.found() or lua51_dep.found()) and lua_lgi_found
if build_lua51_loader and not lua51_found
  error('Lua51 requested but failed to locate suitable Lua51 and LGI support')
endif

build_python3_loader = get_option('python3')
python3_found = python3_dep.found() and pygobject_dep.found()
if build_python3_loader and not python3_found
  error('python3 requested but both python3 and pygobject are required')
endif

# Replace 'cp' calls
copyfile_script = find_program('copyfile.py')

subdir('libpeas')

if build_gtk_doc == true
  subdir('docs')
endif

subdir('po')

subdir('loaders')

if generate_gir == true
  subdir('tests')
endif

summary = [
  '',
  '------',
  'libpeas @0@ (@1@)'.format(version, api_version),
  '',
  '     Documentation: @0@'.format(build_gtk_doc),
  '     Introspection: @0@'.format(generate_gir),
  '',
  'Loaders:',
  '       GJS support: @0@'.format(build_gjs_loader),
  '   Lua 5.1 support: @0@'.format(build_lua51_loader),
  '  Python 3 support: @0@'.format(build_python3_loader),
  '',
  'Directories:',
  '            prefix: @0@'.format(prefix),
  '        includedir: @0@'.format(includedir),
  '            libdir: @0@'.format(libdir),
  '------',
]

message('\n'.join(summary))
