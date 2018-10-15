# NMake Makefile portion for displaying config info

WITH_GTK = no

!if "$(PEAS_NO_GTK)" == ""
WITH_GTK = yes
!endif

build-info-libpeas:
	@echo.
	@echo =========================
	@echo Configuration for libpeas
	@echo =========================
	@echo peas-gtk built: $(WITH_GTK)
	@echo Loaders built: $(PEAS_LOADERS_BUILT)

all-build-info: build-info-libpeas

help:
	@echo.
	@echo ============================
	@echo Building libpeas Using NMake
	@echo ============================
	@echo nmake /f Makefile.vc CFG=[release^|debug] ^<PREFIX=PATH^> ^<PYTHON=PATH^>
	@echo.
	@echo Where:
	@echo ------
	@echo CFG: Required, use CFG=release for an optimized build and CFG=debug
	@echo for a debug build.  PDB files are generated for all builds.
	@echo.
	@echo PYTHON (see this also for PYTHON2/PYTHON3): Required when using
	@echo GLib-2.53.4 or later, and when building the Python2/3 loaders, unless
	@echo python.exe is already in your PATH.  Use PYTHON3=xxx with PYTHON=xxx
	@echo (or PYTHON2=xxx) if building the Python2 and Python 3 loaders
	@echo simultaneously.  Set to ^$(FULL_PATH_TO_PYTHON_INTERPRETOR).
	@echo For the Python loaders, a corresponding installation of PyGObject is
	@echo required.  For building the introspection files, this is also required,
	@echo which means PYTHON must be set to the python.exe which corresponds to
	@echo the Python installation that was used to build gobject-introspection--
	@echo if python is not in your PATH and PYTHON is not set, PYTHON would be
	@echo set as PYTHON3 or PYTHON2 (if any or both are set), in this order.
	@echo.
	@echo LUA: Required for building the lua loader, unless lua.exe is already
	@echo in your PATH, which also requires lgi to be installed.  LUA 5.1.x-5.3.x
	@echo are supported.
	@echo.
	@echo PREFIX: Optional, the path where dependent libraries and tools may be
	@echo found, default is ^$(srcrootdir)\..\vs^$(short_vs_ver)\^$(platform),
	@echo where ^$(short_vs_ver) is 9 for VS 2008, 10 for VS 2010 and so on; and
	@echo ^$(platform) is Win32 for 32-bit builds and x64 for x64 builds.
	@echo ======
	@echo A 'clean' target is supported to remove all generated files, intermediate
	@echo object files and binaries for the specified configuration.
	@echo.
	@echo An 'install' target is supported to copy the build to appropriate
	@echo locations under ^$(PREFIX).
	@echo.
	@echo An 'pre-run-demo' target is supported to copy the build to appropriate
	@echo locations in the source/build tree so that one can run peas-demo using
	@echo "peas-demo --run-from-build-dir".
	@echo ======
	@echo.
	
