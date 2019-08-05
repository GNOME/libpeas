extension_python_name = 'extension-python'

python2_in = configuration_data()
python2_in.set('PY_LOADER', 'python')

configure_file(
  input: 'extension-py.gschema.xml.in',
  output: 'extension-python.gschema.xml',
  configuration: python2_in,
)

configure_file(
  input: 'extension-py.plugin.in',
  output: 'extension-python.plugin',
  configuration: python2_in,
)

configure_file(
  input: 'extension-py.py.in',
  output: 'extension-python.py',
  configuration: python2_in,
)

python3_in = configuration_data()
python3_in.set('PY_LOADER', 'python3')

configure_file(
  input: 'extension-py.gschema.xml.in',
  output: 'extension-python3.gschema.xml',
  configuration: python3_in,
)

configure_file(
  input: 'extension-py.plugin.in',
  output: 'extension-python3.plugin',
  configuration: python3_in,
)

configure_file(
  input: 'extension-py.py.in',
  output: 'extension-python3.py',
  configuration: python3_in,
)
