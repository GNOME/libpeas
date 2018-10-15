# Find out configuration of Python installation
import sys
import platform

def configpy(type):
    # Test whether python is in PATH
    if type == 'testrun':
        print(sys.version)
        return

    # Find Python setup info
    confignmake = open('configpy.mak', 'a')
    pyver = platform.python_version().split('.')
    compiler_details = platform.python_compiler().split()
    compiler_ver = compiler_details[1][2:compiler_details[1].find('00')]

    confignmake.write('PYTHON%sPREFIX=%s\n' % (type, sys.prefix))
    confignmake.write('PYTHON%sMAJ=%s\n' % (type, pyver[0]))
    confignmake.write('PYTHON%sSERIESDOT=%s.%s\n' % (type, pyver[0], pyver[1]))
    try:
        from gi.repository import GLib
        confignmake.write('PYTHON%sPYGOBJECT=1\n' % type)
    except ImportError:
        confignmake.write('PYTHON%sPYGOBJECT=\n' % type)
    confignmake.close()

if __name__ == '__main__':
    if len(sys.argv) > 1:
        configpy(sys.argv[1])
    else:
        configpy('')