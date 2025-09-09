
libpeas_tests_sources = [
  ['engine'],
  ['extension-c'],
  ['extension-set'],
  ['plugin-info'],
]

libpeas_tests_c_args = ['-UG_DISABLE_ASSERT', '-UG_DISABLE_CAST_CHECKS']

if build_gjs_loader == true
  libpeas_tests_sources += [['extension-gjs', [gjs_dep]]]
endif

if build_lua51_loader == true
  warning('Lua51 tests are currently disabled due to LGI lacking girepository-2.0 support')
  # libpeas_tests_sources += [['extension-lua', [lua51_dep, luajit_dep]]]
endif

if build_python3_loader == true
  libpeas_tests_sources += [['extension-py', [python3_dep, pygobject_dep]]]

  libpeas_tests_c_args += [
    '-DPYGOBJECT_MAJOR_VERSION=@0@'.format(pygobject_version_major),
    '-DPYGOBJECT_MINOR_VERSION=@0@'.format(pygobject_version_minor),
    '-DPYGOBJECT_MICRO_VERSION=@0@'.format(pygobject_version_micro),
  ]
endif

subdir('introspection')
subdir('testing')

subdir('plugins')

libpeas_tests_deps = [
  glib_dep,
  libpeas_dep,
  libintrospection_dep,
  libintrospection_gir_dep,
  libtesting_util_dep,
  libpeas_testing_dep,
  libembedded_dep,
]

foreach test: libpeas_tests_sources
  test_name = test.get(0)
  test_sources = ['@0@.c'.format(test_name)]
  test_deps = libpeas_tests_deps

  if test.length() > 1
    test_deps += test.get(1)
  endif

  test_exe = executable(
    test_name,
    test_sources,
    dependencies: test_deps,
    c_args: libpeas_tests_c_args,
  )

  test(
    'test-@0@'.format(test_name),
    test_exe,
    depends: [libintrospection_gir],
    env: test_env,
  )
endforeach
