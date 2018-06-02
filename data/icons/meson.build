icon_sizes = [
  '16x16',
  '22x22',
  '32x32',
]

foreach size: icon_sizes
  install_data(
    join_paths(size, 'libpeas-plugin.png'),
    install_dir: join_paths(
      iconsdir,
      'hicolor',
      size,
      'actions'
    )
  )
endforeach

install_data(
  join_paths('scalable', 'libpeas-plugin.svg'),
  install_dir: join_paths(
    iconsdir,
    'hicolor',
    'scalable',
    'actions'
  )
)
