# NMake Makefile portion for compilation rules
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.  The format
# of NMake Makefiles here are different from the GNU
# Makefiles.  Please see the comments about these formats.

# Inference rules for compiling the .obj files.
# Used for libs and programs with more than a single source file.
# Format is as follows
# (all dirs must have a trailing '\'):
#
# {$(srcdir)}.$(srcext){$(destdir)}.obj::
# 	$(CC)|$(CXX) $(cflags) /Fo$(destdir) /c @<<
# $<
# <<
{..\libpeas\}.c{$(CFG)\$(PLAT)\peas\}.obj::
	$(CC) $(CFLAGS) $(PEAS_LIB_DEFINES) $(PEAS_CFLAGS) /Fo$(CFG)\$(PLAT)\peas\ /c @<<
$<
<<

{$(CFG)\$(PLAT)\peas\}.c{$(CFG)\$(PLAT)\peas\}.obj::
	$(CC) $(CFLAGS) $(PEAS_LIB_DEFINES) $(PEAS_CFLAGS) /Fo$(CFG)\$(PLAT)\peas\ /c @<<
$<
<<

{..\libpeas-gtk\}.c{$(CFG)\$(PLAT)\peas-gtk\}.obj::
	$(CC) $(CFLAGS) $(PEAS_LIB_DEFINES) $(PEAS_GTK_CFLAGS) /Fo$(CFG)\$(PLAT)\peas-gtk\ /c @<<
$<
<<

{..\loaders\python\}.c{$(CFG)\$(PLAT)\loaders-py\}.obj::
	$(CC) $(CFLAGS) $(PEAS_DEFINES) $(PYTHON_LOADER_CFLAGS) /Fo$(CFG)\$(PLAT)\loaders-py\ /c @<<
$<
<<

{..\loaders\python3\}.c{$(CFG)\$(PLAT)\loaders-py3\}.obj::
	$(CC) $(CFLAGS) $(PEAS_DEFINES) $(PYTHON3_LOADER_CFLAGS) /Fo$(CFG)\$(PLAT)\loaders-py3\ /c @<<
$<
<<

{..\loaders\python\}.c{$(CFG)\$(PLAT)\loaders-py3\}.obj::
	$(CC) $(CFLAGS) $(PEAS_DEFINES) $(PYTHON3_LOADER_CFLAGS) /Fo$(CFG)\$(PLAT)\loaders-py3\ /c @<<
$<
<<

{..\loaders\lua5.1\}.c{$(CFG)\$(PLAT)\loaders-lua\}.obj::
	$(CC) $(CFLAGS) $(PEAS_DEFINES) $(LUA_CFLAGS) /Fo$(CFG)\$(PLAT)\loaders-lua\ /c @<<
$<
<<

{$(CFG)\$(PLAT)\loaders-py\}.c{$(CFG)\$(PLAT)\loaders-py\}.obj::
	$(CC) $(CFLAGS) $(PYTHON_LOADER_CFLAGS) /Fo$(CFG)\$(PLAT)\loaders-py\ /c @<<
$<
<<

{$(CFG)\$(PLAT)\loaders-py3\}.c{$(CFG)\$(PLAT)\loaders-py3\}.obj::
	$(CC) $(CFLAGS) $(PYTHON3_LOADER_CFLAGS) /Fo$(CFG)\$(PLAT)\loaders-py3\ /c @<<
$<
<<

{$(CFG)\$(PLAT)\loaders-lua\}.c{$(CFG)\$(PLAT)\loaders-lua\}.obj::
	$(CC) $(CFLAGS) $(LUA_CFLAGS) /Fo$(CFG)\$(PLAT)\loaders-lua\ /c @<<
$<
<<

# Inference rules for demos
{..\peas-demo\}.c{$(CFG)\$(PLAT)\peas-demo\}.obj::
	$(CC) $(CFLAGS) $(PEAS_GTK_CFLAGS) $(PEAS_DEFINES)	\
	/DPEAS_BUILDDIR=\"$(PEAS_BUILDDIR)\"	\
	/Fo$(CFG)\$(PLAT)\peas-demo\ /c @<<
$<
<<

{..\peas-demo\plugins\helloworld\}.c{$(CFG)\$(PLAT)\peas-demo-helloworld\}.obj::
	$(CC) $(CFLAGS) $(PEAS_DEFINES) $(PEAS_GTK_CFLAGS) /Fo$(CFG)\$(PLAT)\peas-demo-helloworld\ /c @<<
$<
<<

{..\peas-demo\plugins\secondtime\}.c{$(CFG)\$(PLAT)\peas-demo-secondtime\}.obj::
	$(CC) $(CFLAGS) $(PEAS_DEFINES) $(PEAS_GTK_CFLAGS) /Fo$(CFG)\$(PLAT)\peas-demo-secondtime\ /c @<<
$<
<<

# Inference rules for building the test programs
# Used for programs with a single source file.
# Format is as follows
# (all dirs must have a trailing '\'):
#
# {$(srcdir)}.$(srcext){$(destdir)}.exe::
# 	$(CC)|$(CXX) $(cflags) $< /Fo$*.obj  /Fe$@ [/link $(linker_flags) $(dep_libs)]

# Rules for linking DLLs
# Format is as follows (the mt command is needed for MSVC 2005/2008 builds):
# $(dll_name_with_path): $(dependent_libs_files_objects_and_items)
#	link /DLL [$(linker_flags)] [$(dependent_libs)] [/def:$(def_file_if_used)] [/implib:$(lib_name_if_needed)] -out:$@ @<<
# $(dependent_objects)
# <<
# 	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
$(CFG)\$(PLAT)\peas-$(APIVERSION).lib: $(PEAS_DLL)
$(CFG)\$(PLAT)\peas-gtk-$(APIVERSION).lib: $(PEAS_GTK_DLL)

$(PEAS_DLL): config.h $(CFG)\$(PLAT)\peas $(PEAS_MARSHAL_SRCS) $(peas_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_DEP_LIBS) -out:$@ -implib:$(CFG)\$(PLAT)\peas-$(APIVERSION).lib @<<
$(peas_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(PEAS_GTK_DLL): $(CFG)\$(PLAT)\peas-$(APIVERSION).lib $(CFG)\$(PLAT)\peas-gtk $(peas_gtk_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(CFG)\$(PLAT)\peas-$(APIVERSION).lib $(PEAS_GTK_LIBS) -out:$@ -implib:$(CFG)\$(PLAT)\peas-gtk-$(APIVERSION).lib @<<
$(peas_gtk_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libpythonloader.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\loaders-py	\
$(CFG)\$(PLAT)\loaders-py\peas-python-resources.c	\
$(peas_py_OBJS)
	link /DLL $(PYTHON_LOADER_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(peas_py_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libpython3loader.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\loaders-py3	\
$(CFG)\$(PLAT)\loaders-py3\peas-python3-resources.c	\
$(peas_py3_OBJS)
	link /DLL $(PYTHON3_LOADER_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(peas_py3_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\liblua51loader.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\loaders-lua	\
$(CFG)\$(PLAT)\loaders-lua\peas-lua-resources.c	\
$(peas_lua_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) lua$(LUAVER).lib $(PEAS_DEP_LIBS) -out:$@ @<<
$(peas_lua_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libhelloworld.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\peas-demo-helloworld	\
$(peas_demo_helloworld_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_GTK_LIBS) -out:$@ @<<
$(peas_demo_helloworld_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libsecondtime.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\peas-demo-secondtime	\
$(peas_demo_secondtime_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_GTK_LIBS) -out:$@ @<<
$(peas_demo_secondtime_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

# Rules for linking EXEs
# Format is as follows (the mt command is needed for MSVC 2005/2008 builds):
# $(exe_name_with_path): $(dependent_libs_files_objects_and_items)
#	link /DLL [$(linker_flags)] [$(dependent_libs)] -out:$@ @<<
# $(dependent_objects)
# <<
# 	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
$(CFG)\$(PLAT)\peas-demo.exe: $(PEAS_LIBS) $(CFG)\$(PLAT)\peas-demo $(peas_demo_OBJS)
	link $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_GTK_LIBS) -out:$@ @<<
$(peas_demo_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
	@-if exist $@.manifest del $@.manifest

clean:
	@for %%x in (dll pdb) do @if exist ..\loaders\python3\libpython3loader.%%x del /f /q ..\loaders\python3\libpython3loader.%%x
	@for %%x in (dll pdb) do @if exist ..\loaders\lua5.1\liblua51loader.%%x del /f /q  ..\loaders\lua5.1\liblua51loader.%%x
	@for %%x in (dll pdb) do @if exist ..\peas-demo\plugins\helloworld\libhelloworld.%%x del /f /q ..\peas-demo\plugins\helloworld\libhelloworld.%%x
	@for %%x in (dll pdb) do @if exist ..\peas-demo\plugins\secondtime\libsecondtime.%%x del /f /q ..\peas-demo\plugins\secondtime\libsecondtime.%%x
	@for %%x in (dll pdb) do @if exist ..\peas-demo\$(PEAS_DLL_NAME).%%x del /f /q ..\peas-demo\$(PEAS_DLL_NAME).%%x
	@for %%x in (dll pdb) do @if exist ..\peas-demo\$(PEAS_GTK_DLL_NAME).%%x del /f /q ..\peas-demo\$(PEAS_GTK_DLL_NAME).%%x
	@for %%x in (exe pdb) do @if exist ..\peas-demo\peas-demo.%%x del /f /q ..\peas-demo\peas-demo.%%x
	@if exist $(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).typelib del $(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).typelib
	@if exist $(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).gir del $(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).gir
	@if exist $(CFG)\$(PLAT)\Peas-$(APIVERSION).typelib del $(CFG)\$(PLAT)\Peas-$(APIVERSION).typelib
	@if exist $(CFG)\$(PLAT)\Peas-$(APIVERSION).gir del $(CFG)\$(PLAT)\Peas-$(APIVERSION).gir
	@-del /f /q $(CFG)\$(PLAT)\*.pdb
	@for %%f in ($(CFG)\$(PLAT)\*.manifest) do @del /f /q $(CFG)\$(PLAT)\%%f
	@-del /f /q $(CFG)\$(PLAT)\*.exe
	@-del /f /q $(CFG)\$(PLAT)\*.dll
	@-del /f /q $(CFG)\$(PLAT)\*.lib
	@-del /f /q $(CFG)\$(PLAT)\*.exp
	@for %%f in ($(CFG)\$(PLAT)\*.ilk) do @del /f /q $(CFG)\$(PLAT)\%%f
	@if exist $(CFG)\$(PLAT)\peas-demo-secondtime\ del /f /q $(CFG)\$(PLAT)\peas-demo-secondtime\*.obj
	@if exist $(CFG)\$(PLAT)\peas-demo-helloworld\ del /f /q $(CFG)\$(PLAT)\peas-demo-helloworld\*.obj
	@if exist $(CFG)\$(PLAT)\peas-demo\ del /f /q $(CFG)\$(PLAT)\peas-demo\*.obj
	@if exist $(CFG)\$(PLAT)\loaders-lua\ del /f /q $(CFG)\$(PLAT)\loaders-lua\*.obj
	@if exist $(CFG)\$(PLAT)\loaders-lua\ del /f /q $(CFG)\$(PLAT)\loaders-lua\peas-lua-resources.c
	@if exist $(CFG)\$(PLAT)\loaders-py3\ del /f /q $(CFG)\$(PLAT)\loaders-py3\*.obj
	@if exist $(CFG)\$(PLAT)\loaders-py3\ del /f /q $(CFG)\$(PLAT)\loaders-py3\peas-python3-resources.c
	@if exist $(CFG)\$(PLAT)\loaders-py\ del /f /q $(CFG)\$(PLAT)\loaders-py\*.obj
	@if exist $(CFG)\$(PLAT)\loaders-py\ del /f /q $(CFG)\$(PLAT)\loaders-py\peas-python-resources.c
	@if exist $(CFG)\$(PLAT)\peas-gtk\ del /f /q $(CFG)\$(PLAT)\peas-gtk\*.obj
	@-del /f /q $(CFG)\$(PLAT)\peas\*.obj
	@-del /f /q $(PEAS_MARSHAL_SRCS)
	@if exist $(CFG)\$(PLAT)\peas-demo-secondtime\ rmdir /s /q $(CFG)\$(PLAT)\peas-demo-secondtime
	@if exist $(CFG)\$(PLAT)\peas-demo-helloworld\ rmdir /s /q $(CFG)\$(PLAT)\peas-demo-helloworld
	@if exist $(CFG)\$(PLAT)\peas-demo\ rmdir /s /q $(CFG)\$(PLAT)\peas-demo
	@if exist $(CFG)\$(PLAT)\loaders-lua\ rmdir /s /q $(CFG)\$(PLAT)\loaders-lua
	@if exist $(CFG)\$(PLAT)\loaders-py3\ rmdir /s /q $(CFG)\$(PLAT)\loaders-py3
	@if exist $(CFG)\$(PLAT)\loaders-py\ rmdir /s /q $(CFG)\$(PLAT)\loaders-py
	@if exist $(CFG)\$(PLAT)\peas-gtk\ rmdir /s /q $(CFG)\$(PLAT)\peas-gtk
	@-rmdir /s /q $(CFG)\$(PLAT)\peas
	@-del /f /q vc$(VSVER)0.pdb

install:
	@if not exist $(PREFIX)\bin mkdir $(PREFIX)\bin
	@if not exist $(PREFIX)\lib mkdir $(PREFIX)\lib
	@if not exist $(PREFIX)\include\libpeas-1.0\libpeas\ mkdir $(PREFIX)\include\libpeas-1.0\libpeas
	@if "$(PEAS_NO_GTK)" == "" if not exist $(PREFIX)\include\libpeas-1.0\libpeas-gtk\ mkdir $(PREFIX)\include\libpeas-1.0\libpeas-gtk
	@for %f in ($(PEAS_DLLS)) do @(copy /b /y %~dpnf.pdb $(PREFIX)\bin\) & (copy /b /y %f $(PREFIX)\bin\)
	@for %f in ($(PEAS_LIBS)) do @copy /b /y %f $(PREFIX)\lib
	@for %h in ($(PEAS_PUBLIC_HDRS)) do @copy /y ..\libpeas\%h $(PREFIX)\include\libpeas-1.0\libpeas
	@if "$(PEAS_NO_GTK)" == "" @(for %h in ($(PEAS_GTK_PUBLIC_HDRS)) do @copy /y ..\libpeas-gtk\%h $(PREFIX)\include\libpeas-1.0\libpeas-gtk\)
	@if not "$(PEAS_LOADERS)" == "" @(if not exist $(PREFIX)\lib\libpeas-1.0\loaders\ mkdir $(PREFIX)\lib\libpeas-1.0\loaders)
	@if not "$(PEAS_LOADERS)" == "" @(for %f in ($(PEAS_LOADERS))do @(copy /b /y %~dpnf.pdb $(PREFIX)\lib\libpeas-1.0\loaders\) & (copy /b /y %f $(PREFIX)\lib\libpeas-1.0\loaders\))
	@for %f in ($(CFG)\$(PLAT)\peas-demo.exe) do @(if exist %f (copy /b /y %~dpnf.pdb $(PREFIX)\bin\) & (copy /b /y %f $(PREFIX)\bin\))
	@for %f in ($(CFG)\$(PLAT)\libhelloworld.dll) do @(if exist %f (if not exist $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\helloworld\ mkdir $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\helloworld) & (copy /b /y %~dpnf.pdb $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\helloworld\) & (copy /b /y %f $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\helloworld\) & (copy /y ..\peas-demo\plugins\helloworld\helloworld.plugin $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\helloworld\))
	@for %f in ($(CFG)\$(PLAT)\libsecondtime.dll) do @(if exist %f (if not exist $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\secondtime\ mkdir $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\secondtime) & (copy /b /y %~dpnf.pdb $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\secondtime\) & (copy /b /y %f $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\secondtime\) & (copy /y ..\peas-demo\plugins\secondtime\secondtime.plugin $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\secondtime\))
	@for %f in ($(CFG)\$(PLAT)\libpython3loader.dll) do @(if exist %f (if not exist $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\pythonhello\ mkdir $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\pythonhello) & (copy /y ..\peas-demo\plugins\pythonhello\pythonhello.py $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\pythonhello\) & (copy /y ..\peas-demo\plugins\pythonhello\pythonhello.plugin $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\pythonhello\))
	@for %f in ($(CFG)\$(PLAT)\liblua51loader.dll) do @(if exist %f (if not exist $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\luahello\ mkdir $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\luahello) & (copy /y ..\peas-demo\plugins\luahello\luahello.lua $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\luahello\) & (copy /y ..\peas-demo\plugins\luahello\luahello.plugin $(PREFIX)\lib\libpeas-1.0\peas-demo\plugins\luahello\))
	@if exist $(CFG)\$(PLAT)\Peas-$(APIVERSION).gir (if not exist $(PREFIX)\share\gir-1.0\ mkdir $(PREFIX)\share\gir-1.0) & (copy /y $(CFG)\$(PLAT)\Peas-$(APIVERSION).gir $(PREFIX)\share\gir-1.0\)
	@if exist $(CFG)\$(PLAT)\Peas-$(APIVERSION).typelib (if not exist $(PREFIX)\lib\girepository-1.0\ mkdir $(PREFIX)\lib\girepository-1.0) & (copy /y $(CFG)\$(PLAT)\Peas-$(APIVERSION).typelib $(PREFIX)\lib\girepository-1.0\)
	@if exist $(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).gir (copy /y $(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).gir $(PREFIX)\share\gir-1.0\)
	@if exist $(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).typelib (copy /y $(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).typelib $(PREFIX)\lib\girepository-1.0\)

prep-run-demo:
	@if not exist $(CFG)\$(PLAT)\peas-demo.exe (echo peas-demo is not built yet!) & (goto :eof)
	@for %%x in (exe pdb) do @copy /b /y $(CFG)\$(PLAT)\peas-demo.%%x ..\peas-demo
	@for %%x in (dll pdb) do @copy /b /y $(CFG)\$(PLAT)\$(PEAS_GTK_DLL_NAME).%%x ..\peas-demo
	@for %%x in (dll pdb) do @copy /b /y $(CFG)\$(PLAT)\$(PEAS_DLL_NAME).%%x ..\peas-demo
	@for %%x in (dll pdb) do @copy /b /y $(CFG)\$(PLAT)\libhelloworld.%%x ..\peas-demo\plugins\helloworld
	@for %%x in (dll pdb) do @copy /b /y $(CFG)\$(PLAT)\libsecondtime.%%x ..\peas-demo\plugins\secondtime
	@for %%x in (dll pdb) do @if exist $(CFG)\$(PLAT)\libpython3loader.%%x copy /b /y $(CFG)\$(PLAT)\libpython3loader.%%x ..\loaders\python3
	@for %%x in (dll pdb) do @if exist $(CFG)\$(PLAT)\liblua51loader.%%x copy /b /y $(CFG)\$(PLAT)\liblua51loader.%%x ..\loaders\lua5.1
	@echo Please set the environment variables as follows to run the peas-demo program
	@echo directly from the build:
	@echo set GI_TYPELIB_PATH=%%CD%%\$(CFG)\$(PLAT)
	@echo Ensure that the lgi files can be loaded by your Lua installation
	@echo Go to ..\peas-demo and run "peas-demo.exe --run-from-build-dir"