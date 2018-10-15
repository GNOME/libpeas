# NMake Makefile portion for enabling features for Windows builds

# You may change these lines to customize the .lib files that will be linked to

# gobject-introspection, GLib and libffi are required for everything

PEAS_BASE_LIBS = \
	girepository-1.0.lib	\
	gio-2.0.lib		\
	gobject-2.0.lib		\
	glib-2.0.lib		\
	
PEAS_DEP_LIBS =	\
	gmodule-2.0.lib		\
	$(PEAS_BASE_LIBS)	\
	intl.lib

PEAS_GTK_LIBS =	\
	gtk-3.0.lib	\
	gdk-3.0.lib	\
	$(PEAS_BASE_LIBS)

# Please do not change anything beneath this line unless maintaining the NMake Makefiles

APIVERSION = 1.0

# Get libpeas basedir, in forward-slash form
!if [echo PEAS_BUILDDIR_DOS=%CD%\.. > peasdir.x]
!endif

!include peasdir.x

!if [del /f peasdir.x]
!endif

PEAS_BUILDDIR = $(PEAS_BUILDDIR_DOS:\=/)

# Configure for built loaders
PEAS_LOADERS =
PEAS_LOADERS_BUILT =

# We include pythonconfig.mak in Makefile.vc because
# we want to check on Python before setting PYTHON as
# 'python' unconditionally
!include luaconfig.mak

PEAS_DEFINES =		\
	/DHAVE_CONFIG_H	\
	/FImsvc_recommended_pragmas.h

PEAS_LIB_DEFINES =	\
	$(PEAS_DEFINES)	\
	/D_PEAS_EXTERN=__declspec(dllexport)extern

PEAS_BASE_INCLUDES =	\
	/I$(PREFIX)\include\gobject-introspection-1.0\girepository	\
	/I$(PREFIX)\include\glib-2.0	\
	/I$(PREFIX)\lib\glib-2.0\include	\
	/I$(PREFIX)\include

PEAS_CFLAGS =				\
	/I.. 			\
	/I..\libpeas 			\
	/I$(CFG)\$(PLAT)\peas	\
	/I.	\
	$(PEAS_BASE_INCLUDES)

PEAS_GTK_CFLAGS =			\
	/I..			\
	/I..\libpeas-gtk		\
	/I.	\
	/I$(PREFIX)\include\gtk-3.0	\
	/I$(PREFIX)\include\gdk-pixbuf-2.0	\
	/I$(PREFIX)\include\pango-1.0	\
	/I$(PREFIX)\include\atk-1.0		\
	$(PEAS_BASE_INCLUDES)

PYTHON_LOADER_CFLAGS =	\
	/I$(PYTHON2PREFIX)\include\pygobject-3.0	\
	/I$(PYTHON2PREFIX)\include					\
	/DENABLE_PYTHON2	\
	$(PEAS_CFLAGS)

PYTHON3_LOADER_CFLAGS =	\
	/I$(PYTHON3PREFIX)\include\pygobject-3.0	\
	/I$(PYTHON3PREFIX)\include					\
	/DENABLE_PYTHON3	\
	$(PEAS_CFLAGS)

PEAS_LDFLAGS =						\
	$(LDFLAGS)						\
	/libpath:$(PREFIX)\libs

PYTHON_LOADER_LDFLAGS =	\
	/libpath:$(PYTHON2PREFIX)\libs	\
	$(PEAS_LDFLAGS)

PYTHON3_LOADER_LDFLAGS =	\
	/libpath:$(PYTHON3PREFIX)\libs	\
	$(PEAS_LDFLAGS)

!if "$(ADDITIONAL_LIB_DIR)" != ""
PEAS_LDFLAGS = /libpath:$(ADDITIONAL_LIB_DIR) $(PYGI_LDFLAGS)
!endif

!if "$(USE_LIBTOOL_DLL_NAMES)" == "1"
PEAS_DLL_NAME = libpeas-1-0
PEAS_GTK_DLL_NAME = libpeas-gtk-1-0
!else
PEAS_DLL_NAME = peas-1-vs$(VSVER)
PEAS_GTK_DLL_NAME = peas-gtk-1-vs$(VSVER)
!endif

PEAS_DLL = $(CFG)\$(PLAT)\$(PEAS_DLL_NAME).dll
PEAS_GTK_DLL = $(CFG)\$(PLAT)\$(PEAS_GTK_DLL_NAME).dll

PEAS_DLLS = $(PEAS_DLL)
PEAS_LIBS = $(CFG)\$(PLAT)\peas-$(APIVERSION).lib
PEAS_DEMOS =

!if "$(PEAS_NO_GTK)" == ""
PEAS_DLLS = $(PEAS_DLLS) $(PEAS_GTK_DLL)
PEAS_LIBS = $(PEAS_LIBS) $(CFG)\$(PLAT)\peas-gtk-$(APIVERSION).lib
PEAS_DEMOS =	\
	$(CFG)\$(PLAT)\peas-demo.exe $(PEAS_DEMOS)	\
	$(CFG)\$(PLAT)\libhelloworld.dll	\
	$(CFG)\$(PLAT)\libsecondtime.dll
!endif

!if "$(BUILD_PY2_LOADER)" == "1"
PEAS_LOADERS = $(CFG)\$(PLAT)\libpythonloader.dll $(PEAS_LOADERS)
PEAS_LOADERS_BUILT = python2 $(PEAS_LOADERS_BUILT)
!endif
!if "$(BUILD_PY3_LOADER)" == "1"
PEAS_LOADERS = $(CFG)\$(PLAT)\libpython3loader.dll $(PEAS_LOADERS)
PEAS_LOADERS_BUILT = python3 $(PEAS_LOADERS_BUILT)
!endif

!if "$(LGI_ERR)" == "0"
PEAS_LOADERS = $(CFG)\$(PLAT)\liblua51loader.dll $(PEAS_LOADERS)
PEAS_LOADERS_BUILT = lua $(PEAS_LOADERS_BUILT)

LUA_CFLAGS = $(PEAS_DEFINES) $(PEAS_CFLAGS)
!if "$(LUA_EXTRA_CFLAGS)" != ""
LUA_CFLAGS = $(LUA_EXTRA_CFLAGS) $(LUA_CFLAGS)
!endif
!endif