
libpeas_gtk_tests_sources = [
  ['plugin-manager'],
  ['plugin-manager-store'],
  ['plugin-manager-view'],
]

subdir('testing')

subdir('plugins')

libpeas_gtk_tests_deps = [
  glib_dep,
  gtk_dep,
  libpeas_dep,
  libpeas_gtk_dep,

  libtesting_util_dep,
  libpeas_gtk_testing_dep,
]

foreach test: libpeas_gtk_tests_sources
  test_name = test.get(0)
  test_sources = ['@0@.c'.format(test_name)]

  test_exe = executable(
    test_name,
    test_sources,
    c_args: ['-UG_DISABLE_ASSERT', '-UG_DISABLE_CAST_CHECKS'],
    dependencies: libpeas_gtk_tests_deps
  )

  test('test-@0@'.format(test_name), test_exe, env: test_env)
endforeach
