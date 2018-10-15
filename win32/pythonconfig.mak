# Only test Python stuff with a valid config
!if "$(CFG)" == "Release" || "$(CFG)" == "release" || "$(CFG)" == "Debug" || "$(CFG)" == "debug"

# Do not set PYTHON as "python" automatically,
# unless python.exe is in PATH
!if "$(PYTHON)" == ""

# Test run Python to see whether it is in PATH
# by printing out version info, and set PYTHON=python
# if it is found in PATH
!if [python configpy.py testrun > pythonrun.xx]
!endif

!if [for %p in (pythonrun.xx) do @(if %~zp GTR 0 echo PYTHON=python > pyexe.mak) & @(del /f pythonrun.xx)]
!endif

!if exist (pyexe.mak)
!include pyexe.mak
!if [del /f pyexe.mak]
!endif
!endif
!endif

# If PYTHON3 or PYTHON2 is set, set PYTHON
# in that order, if PYTHON is not set
!if "$(PYTHON)" == ""
!if "$(PYTHON3)" != ""
PYTHON = $(PYTHON3)
!elseif "$(PYTHON2)" != ""
PYTHON = $(PYTHON2)
!else
# Python is not in PATH and PYTHON2 and PYTHON3 are not set
PYTHON = xxx
!endif
!endif

!if "$(PYTHON)" != "xxx"
!if [echo PYTHON_CONFIG_FILE= > configpy.mak]
!endif

!if [$(PYTHON) configpy.py"]
!endif
!endif

!if "$(PYTHON3)" != ""
!if [$(PYTHON3) configpy.py 3]
!endif
!endif

!if "$(PYTHON2)" != ""
!if [$(PYTHON2) configpy.py 2]
!endif
!endif

!if exist (configpy.mak)
!include configpy.mak

!if ![del /f /q configpy.mak]
!endif
!endif

# Set up command to call glib-genmarshal, which may
# be a python script or a .exe binary
!if exist ($(PREFIX)\bin\glib-genmarshal)
!if "$(PYTHON)" != "xxx"
GLIB_GENMARSHAL_CMD = $(PYTHON) $(PREFIX)\bin\glib-genmarshal
!else
GLIB_GENMARSHAL_CMD = $(PREFIX)\bin\glib-genmarshal.exe
!endif
!else
GLIB_GENMARSHAL_CMD = $(PREFIX)\bin\glib-genmarshal.exe
!endif

!if "$(GLIB_GENMARSHAL_CMD)" == "$(PREFIX)\bin\glib-genmarshal.exe"
!if !exist ($(GLIB_GENMARSHAL_CMD))
!message Warning: Could not run glib-genmarshal.  If building, the build
!message will most probably fail after seeing this message.  Please
!message ensure the following:
!message -glib-genmarshal or glib-genmarshal.exe can be found at $(PREFIX)\bin,
!message or set PREFIX so that it can be found at <PREFIX>\bin.
!message -If using glib-genmarshal as a Python script, ensure that
!message PYTHON3 or PYTHON2 or PYTHON is set correctly, or python.exe
!message is in your PATH.
!endif
!endif

# Determine installation prefix and existance of
# PyGObject 3.x in that Python installation
!if "$(PYTHONMAJ)" == "2"
!if "$(PYTHON2PREFIX)" == ""
PYTHON2MAJ = $(PYTHONMAJ)
PYTHON2PREFIX = $(PYTHONPREFIX)
PYTHON2PYGOBJECT = $(PYTHONPYGOBJECT)
!endif
!endif

!if "$(PYTHONMAJ)" == "3"
!if "$(PYTHON3PREFIX)" == ""
PYTHON3MAJ = $(PYTHONMAJ)
PYTHON3PREFIX = $(PYTHONPREFIX)
PYTHON3PYGOBJECT = $(PYTHONPYGOBJECT)
!endif
!endif

# Now, determine which Python loader(s) will be built
!if "$(PYTHON2PYGOBJECT)" == "1"
!if exist($(PYTHON2PREFIX)\include\pygobject-3.0\pygobject.h)
BUILD_PY2_LOADER=1
!endif
!endif

!if "$(PYTHON3PYGOBJECT)" == "1"
!if exist($(PYTHON3PREFIX)\include\pygobject-3.0\pygobject.h)
BUILD_PY3_LOADER=1
!endif
!endif
!endif