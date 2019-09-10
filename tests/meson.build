test_env = [
  'G_DEBUG=gc-friendly',
  'GSETTINGS_BACKEND=memory',
  'MALLOC_CHECK_=2',
  'NO_AT_BRIDGE=1',
]

subdir('testing-util')

subdir('libpeas')
if build_gtk_widgetry == true
  subdir('libpeas-gtk')
endif

subdir('plugins')
