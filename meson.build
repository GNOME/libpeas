project(
  'libpeas', 'c',
  version: '1.30.0',
  license: 'LGPLv2.1+',
  meson_version: '>= 0.50.0',
  default_options: [
    'buildtype=debugoptimized',
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

api_version = '@0@.0'.format(version_major)

if version_minor.is_odd()
  interface_age = 0
else
  interface_age = version_micro
endif

# Compatibility with libtool versioning:
#  current = minor * 100 + micro - interface
#  revision = interface
lib_version_soversion = 0
lib_version_current = version_minor * 100 + version_micro - interface_age
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
gtkdocdir = join_paths(datadir, 'gtk-doc')
iconsdir = join_paths(datadir, 'icons')
pkgconfigdir = join_paths(libdir, 'pkgconfig')
typelibdir = join_paths(libdir, 'girepository-1.0')

# Configurations
config_h = configuration_data()
config_h.set_quoted('PACKAGE', package_name)
config_h.set_quoted('PACKAGE_BUGREPORT', 'http://bugzilla.gnome.org/enter_bug.cgi?product=libpeas')
config_h.set_quoted('PACKAGE_NAME', package_name)
config_h.set_quoted('PACKAGE_STRING', package_long_string)
config_h.set_quoted('PACKAGE_URL', 'https://wiki.gnome.org/Projects/Libpeas')
config_h.set_quoted('PACKAGE_URL', 'https://wiki.gnome.org/Projects/Libpeas')
config_h.set_quoted('PACKAGE_VERSION', version)
config_h.set_quoted('VERSION', version)

config_h.set_quoted('GETTEXT_PACKAGE', package_string)

# Dependencies
cc = meson.get_compiler('c')

glib_req_version = '2.38.0'

glib_req = '>= @0@'.format(glib_req_version)
introspection_req = '>= 1.39.0'
gtk_req = '>= 3.0.0'
gtk_doc_req = '>= 1.11'
python2_req = '>= 2.5.2'
python3_req = '>= 3.2.0'
pygobject_req = '>= 3.2.0'
lua_req = '>= 5.1.0'
lua_lgi_req = '>= 0.9.0'
luajit_req = '>= 2.0'

glib_dep = dependency('glib-2.0', version: glib_req)
gobject_dep = dependency('gobject-2.0', version: glib_req)
gmodule_dep = dependency('gmodule-2.0', version: glib_req)
gio_dep = dependency('gio-2.0', version: glib_req)
introspection_dep = dependency('gobject-introspection-1.0', version: introspection_req)
gtk_dep = dependency('gtk+-3.0', version: gtk_req, required: false)

gtk_doc_dep = dependency('gtk-doc', version: gtk_doc_req, required: false)
gladeui_dep = dependency('gladeui-2.0', required: false)

# From python 3.8 we neeed python3-embed
python3_dep = dependency('python3-embed', required: false)
if not python3_dep.found()
  python3_dep = dependency('python3', version: python3_req, required: false)
endif

if host_machine.system() == 'windows' and not python3_dep.found()
  python3 = pymod.find_installation(get_option('python3_path'),
                                    required: false)
  if python3.found()
    python3_dep = python3.dependency(version: python3_req, required: false)
  endif
endif


python2_dep = dependency('python2', version: python2_req, required: false)
if host_machine.system() == 'windows' and not python2_dep.found()
  python2 = pymod.find_installation(get_option('python2_path') != '' ? get_option('python2_path') : 'python2',
                                    required: false)
  if python2.found()
    python2_dep = python2.dependency(version: python2_req, required: false)
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

if cc.get_id() == 'msvc'
  if luajit_prg.found()
    # luajit has lua51.lib as its import library
    lua_names = ['lua51']
  else
    lua_names = ['lua53', 'lua52', 'lua51']
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
    lua_lgi_check = run_command(lua_prg, ['-e', 'print(require("lgi")._VERSION)'])
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

add_project_arguments([
  '-DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_@0@_@1@'.format(glib_major_version, glib_minor_version),
  '-DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_@0@_@1@'.format(glib_major_version, glib_minor_version),
], language: 'c')

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
build_gtk_doc = get_option('gtk_doc')
if build_gtk_doc and not gtk_doc_dep.found()
  build_gtk_doc = false
endif

install_glade_catalog = get_option('glade_catalog')
if install_glade_catalog and not gladeui_dep.found()
  install_glade_catalog = false
elif install_glade_catalog == true
  catalogdir = join_paths(datadir, 'glade', 'catalogs')
# FIXME: In gladeui-2.0.pc, catalogdir should be relative to datadir:
# https://gitlab.gnome.org/GNOME/glade/issues/353
#   catalogdir = gladeui_dep.get_pkgconfig_variable(
#     'catalogdir',
#     define_variable: [
#       'datadir', datadir,
#     ]
#   )
endif

build_gtk_widgetry = get_option('widgetry')
if build_gtk_widgetry and not gtk_dep.found()
  build_gtk_widgetry = false
endif

build_demos = get_option('demos')
if build_demos and not build_gtk_widgetry
  build_demos = false
endif

generate_gir = get_option('introspection')
if generate_gir and not introspection_dep.found()
  generate_gir = false
endif

build_lua51_loader = get_option('lua51')
lua51_found = (luajit_dep.found() or lua51_dep.found()) and lua_lgi_found
if build_lua51_loader and not lua51_found
  build_lua51_loader = false
endif

build_python2_loader = get_option('python2')
python2_found = python2_dep.found() and pygobject_dep.found()
if build_python2_loader and not python2_found
  build_python2_loader = false
endif

build_python3_loader = get_option('python3')
python3_found = python3_dep.found() and pygobject_dep.found()
if build_python3_loader and not python3_found
  build_python3_loader = false
endif

# Replace 'cp' calls
copyfile_script = find_program('copyfile.py')

subdir('libpeas')
if build_gtk_widgetry == true
  subdir('libpeas-gtk')
endif

subdir('docs')
subdir('data')
subdir('po')

subdir('loaders')

if build_demos == true
  subdir('peas-demo')
endif
if generate_gir == true
  subdir('tests')
endif

summary = [
  '',
  '------',
  'libpeas @0@ (@1@)'.format(version, api_version),
  '',
  '             Demos: @0@'.format(build_demos),
  '     Documentation: @0@'.format(build_gtk_doc),
  '     Glade catalog: @0@'.format(install_glade_catalog),
  '     GTK+ widgetry: @0@'.format(build_gtk_widgetry),
  '     Introspection: @0@'.format(generate_gir),
  '   Lua 5.1 support: @0@'.format(build_lua51_loader),
  '  Python 2 support: @0@'.format(build_python2_loader),
  '  Python 3 support: @0@'.format(build_python3_loader),
  '',
  'Directories:',
  '            prefix: @0@'.format(prefix),
  '        includedir: @0@'.format(includedir),
  '            libdir: @0@'.format(libdir),
  '           datadir: @0@'.format(datadir),
  '------',
]

message('\n'.join(summary))
