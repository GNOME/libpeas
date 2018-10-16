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

# Inference rules for the various test sources

{..\tests\testing-util\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_CFLAGS) $(PEAS_LIB_DEFINES) $(TEST_UTIL_CFLAGS) $(TEST_UTIL_DEFINES) /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

{..\tests\libpeas\testing\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_CFLAGS) $(PEAS_LIB_DEFINES) $(TEST_UTIL_CFLAGS) $(TEST_UTIL_DEFINES) /I..\tests\libpeas\introspection /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

{..\tests\libpeas\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_CFLAGS) $(PEAS_LIB_DEFINES) $(TEST_UTIL_CFLAGS) /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

{..\tests\libpeas-gtk\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_GTK_CFLAGS) $(PEAS_LIB_DEFINES) $(TEST_GTK_UTIL_CFLAGS) /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

{..\tests\libpeas\introspection\}.c{$(CFG)\$(PLAT)\peas-test-introspection\}.obj::
	$(CC) $(CFLAGS) $(PEAS_CFLAGS) $(PEAS_LIB_DEFINES) /Fo$(CFG)\$(PLAT)\peas-test-introspection\ /c @<<
$<
<<

{..\tests\libpeas\plugins\embedded\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_CFLAGS) $(PEAS_LIB_DEFINES) /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

{..\tests\libpeas\plugins\extension-c\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_CFLAGS) $(PEAS_LIB_DEFINES) /I..\tests\libpeas\introspection /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

{..\tests\plugins\builtin\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_CFLAGS) $(PEAS_LIB_DEFINES) /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

{..\tests\plugins\has-dep\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_CFLAGS) $(PEAS_LIB_DEFINES) /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

{..\tests\plugins\loadable\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_CFLAGS) $(PEAS_LIB_DEFINES) /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

{..\tests\plugins\self-dep\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_CFLAGS) $(PEAS_LIB_DEFINES) /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

{..\tests\libpeas-gtk\plugins\builtin-configurable\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_GTK_CFLAGS) $(PEAS_LIB_DEFINES) /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

{..\tests\libpeas-gtk\plugins\configurable\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_GTK_CFLAGS) $(PEAS_LIB_DEFINES) /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

{$(CFG)\$(PLAT)\peas-test\}.c{$(CFG)\$(PLAT)\peas-test\}.obj::
	$(CC) $(CFLAGS) $(PEAS_CFLAGS) $(PEAS_LIB_DEFINES) /Fo$(CFG)\$(PLAT)\peas-test\ /c @<<
$<
<<

$(CFG)\$(PLAT)\peas-test\extension-python.obj: ..\tests\libpeas\extension-py.c
	$(CC) $(CFLAGS) $(PEAS_DEFINES) $(TEST_UTIL_CFLAGS) $(PYTHON_EXT_TEST_CFLAG) /Fo$@ $** /c

$(CFG)\$(PLAT)\peas-test\extension-python3.obj: ..\tests\libpeas\extension-py.c
	$(CC) $(CFLAGS) $(PEAS_DEFINES) $(TEST_UTIL_CFLAGS) $(PYTHON3_EXT_TEST_CFLAG) /Fo$@ $** /c

$(CFG)\$(PLAT)\peas-test\testing-gtk.obj: ..\tests\libpeas-gtk\testing\testing.c
	$(CC) $(CFLAGS) $(PEAS_GTK_CFLAGS) $(PEAS_DEFINES) $(TEST_UTIL_DEFINES) $(TEST_GTK_UTIL_CFLAGS) /Fo$@ $** /c

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
$(CFG)\$(PLAT)\introspection.lib: $(CFG)\$(PLAT)\introspection.dll

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

$(CFG)\$(PLAT)\introspection.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\peas-test-introspection	\
$(peas_test_introspection_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(peas_test_introspection_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libembedded.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\peas-test	\
$(peas_test_embedded_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(peas_test_embedded_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libloadable.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\peas-test	\
$(peas_test_loadable_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(peas_test_loadable_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libbuiltin.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\peas-test	\
$(peas_test_builtin_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(peas_test_builtin_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libhas-dep.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\peas-test	\
$(peas_test_has_dep_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(peas_test_has_dep_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libself-dep.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\peas-test	\
$(peas_test_self_dep_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(peas_test_self_dep_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libextension-c.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\peas-test	\
$(CFG)\$(PLAT)\Introspection-1.0.typelib	\
$(peas_test_extension_c_mod_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) $(CFG)\$(PLAT)\introspection.lib -out:$@ @<<
$(peas_test_extension_c_mod_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libextension-c-missing-symbol.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\peas-test	\
$(peas_test_extension_c_missing_symbol_mod_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(peas_test_extension_c_missing_symbol_mod_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libbuiltin-configurable.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\peas-test	\
$(peas_test_gtk_builtin_configurable_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_GTK_LIBS) -out:$@ @<<
$(peas_test_gtk_builtin_configurable_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\libconfigurable.dll:	\
$(PEAS_LIBS)	\
$(CFG)\$(PLAT)\peas-test	\
$(peas_test_gtk_configurable_OBJS)
	link /DLL $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_GTK_LIBS) -out:$@ @<<
$(peas_test_gtk_configurable_OBJS)
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

$(CFG)\$(PLAT)\engine.exe:	\
$(CFG)\$(PLAT)\peas-test	\
$(CFG)\$(PLAT)\Introspection-1.0.typelib	\
$(TESTS_DLLS)	\
$(TESTING_UTIL_OBJS)	\
$(CFG)\$(PLAT)\peas-test\engine.obj
	link $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(CFG)\$(PLAT)\peas-test\engine.obj $(TESTING_UTIL_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\extension-c.exe:	\
$(CFG)\$(PLAT)\peas-test	\
$(CFG)\$(PLAT)\Introspection-1.0.typelib	\
$(TESTS_DLLS)	\
$(TESTING_EXT_UTIL_OBJS)	\
$(CFG)\$(PLAT)\libembedded.dll	\
$(CFG)\$(PLAT)\peas-test\extension-c.obj
	link $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) $(CFG)\$(PLAT)\introspection.lib -out:$@ @<<
$(CFG)\$(PLAT)\peas-test\extension-c.obj $(TESTING_EXT_UTIL_OBJS) $(CFG)\$(PLAT)\peas-test\embedded-resources.obj $(CFG)\$(PLAT)\libembedded.lib
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\extension-lua.exe:	\
$(CFG)\$(PLAT)\peas-test	\
$(CFG)\$(PLAT)\liblua51loader.dll	\
$(TESTS_DLLS)	\
$(TESTING_EXT_UTIL_OBJS)	\
$(CFG)\$(PLAT)\peas-test\extension-lua.obj
	link $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) $(CFG)\$(PLAT)\introspection.lib -out:$@ @<<
$(CFG)\$(PLAT)\peas-test\extension-lua.obj $(TESTING_EXT_UTIL_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\extension-python.exe:	\
$(CFG)\$(PLAT)\peas-test	\
$(CFG)\$(PLAT)\libpythonloader.dll	\
$(TESTS_DLLS)	\
$(TESTING_EXT_UTIL_OBJS)	\
$(CFG)\$(PLAT)\peas-test\extension-python.obj
	link $(PYTHON_LOADER_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) $(CFG)\$(PLAT)\introspection.lib -out:$@ @<<
$(CFG)\$(PLAT)\peas-test\extension-python.obj $(TESTING_EXT_UTIL_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\extension-python3.exe:	\
$(CFG)\$(PLAT)\peas-test	\
$(CFG)\$(PLAT)\libpython3loader.dll	\
$(TESTS_DLLS)	\
$(TESTING_EXT_UTIL_OBJS)	\
$(CFG)\$(PLAT)\peas-test\extension-python3.obj
	link $(PYTHON3_LOADER_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) $(CFG)\$(PLAT)\introspection.lib -out:$@ @<<
$(CFG)\$(PLAT)\peas-test\extension-python3.obj $(TESTING_EXT_UTIL_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\extension-set.exe:	\
$(CFG)\$(PLAT)\peas-test	\
$(TESTS_DLLS)	\
$(TESTING_UTIL_OBJS)	\
$(CFG)\$(PLAT)\peas-test\extension-set.obj
	link $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(CFG)\$(PLAT)\peas-test\extension-set.obj $(TESTING_UTIL_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\plugin-info.exe:	\
$(CFG)\$(PLAT)\peas-test	\
$(TESTS_DLLS)	\
$(TESTING_UTIL_OBJS)	\
$(CFG)\$(PLAT)\peas-test\plugin-info.obj
	link $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(CFG)\$(PLAT)\peas-test\plugin-info.obj $(TESTING_UTIL_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\plugin-manager.exe:	\
$(CFG)\$(PLAT)\peas-test	\
$(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).typelib	\
$(TESTS_DLLS)	\
$(TESTING_GTK_UTIL_OBJS)	\
$(CFG)\$(PLAT)\peas-test\plugin-manager.obj
	link $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_GTK_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(CFG)\$(PLAT)\peas-test\plugin-manager.obj $(TESTING_GTK_UTIL_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\plugin-manager-store.exe:	\
$(CFG)\$(PLAT)\peas-test	\
$(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).typelib	\
$(TESTS_DLLS)	\
$(TESTING_GTK_UTIL_OBJS)	\
$(CFG)\$(PLAT)\peas-test\plugin-manager-store.obj
	link $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_GTK_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(CFG)\$(PLAT)\peas-test\plugin-manager-store.obj $(TESTING_GTK_UTIL_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
	@-if exist $@.manifest del $@.manifest

$(CFG)\$(PLAT)\plugin-manager-view.exe:	\
$(CFG)\$(PLAT)\peas-test	\
$(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).typelib	\
$(TESTS_DLLS)	\
$(TESTING_GTK_UTIL_OBJS)	\
$(CFG)\$(PLAT)\peas-test\plugin-manager-view.obj
	link $(PEAS_LDFLAGS) $(PEAS_LIBS) $(PEAS_GTK_LIBS) $(PEAS_DEP_LIBS) -out:$@ @<<
$(CFG)\$(PLAT)\peas-test\plugin-manager-view.obj $(TESTING_GTK_UTIL_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
	@-if exist $@.manifest del $@.manifest

clean:
	@for %%d in (builtin has-dep loadable self-dep) do @(for %%x in (dll pdb) do @if exist ..\tests\plugins\%%d\lib%%d.%%x del /f /q ..\tests\plugins\%%d\lib%%d.%%x)
	@for %%d in (embedded extension-c) do @(for %%x in (dll pdb) do @if exist ..\tests\libpeas\plugins\%%d\lib%%d.%%x del /f /q ..\tests\libpeas\plugins\%%d\lib%%d.%%x)
	@for %%x in (dll pdb) do @if exist ..\tests\libpeas\plugins\extension-c\libextension-c-missing-symbol.%%x del /f /q ..\tests\libpeas\plugins\extension-c\libextension-c-missing-symbol.%%x
	@for %%d in (builtin-configurable configurable) do @(for %%x in (dll pdb) do @if exist ..\tests\libpeas-gtk\plugins\%%d\lib%%d.%%x del /f /q ..\tests\libpeas-gtk\plugins\%%d\lib%%d.%%x)
	@for %%x in (gschema.xml plugin py) do @del /f /q ..\tests\libpeas\plugins\extension-python\*.%%x
	@for %%t in (c lua python) do @if exist ..\tests\libpeas\plugins\extension-%%t\gschema.compiled del /f /q ..\tests\libpeas\plugins\extension-%%t\gschema.compiled
	@if exist ..\tests\libpeas\introspection\Introspection-1.0.typelib del /f /q ..\tests\libpeas\introspection\Introspection-1.0.typelib
	@if exist ..\libpeas-gtk\PeasGtk-$(APIVERSION).typelib del /f /q ..\libpeas-gtk\PeasGtk-$(APIVERSION).typelib
	@if exist ..\libpeas\Peas-$(APIVERSION).typelib del /f /q ..\libpeas\Peas-$(APIVERSION).typelib
	@for %%x in (dll pdb) do @if exist ..\loaders\python3\libpython3loader.%%x del /f /q ..\loaders\python3\libpython3loader.%%x
	@for %%x in (dll pdb) do @if exist ..\loaders\lua5.1\liblua51loader.%%x del /f /q  ..\loaders\lua5.1\liblua51loader.%%x
	@for %%x in (dll pdb) do @if exist ..\peas-demo\plugins\helloworld\libhelloworld.%%x del /f /q ..\peas-demo\plugins\helloworld\libhelloworld.%%x
	@for %%x in (dll pdb) do @if exist ..\peas-demo\plugins\secondtime\libsecondtime.%%x del /f /q ..\peas-demo\plugins\secondtime\libsecondtime.%%x
	@for %%x in (dll pdb) do @if exist ..\peas-demo\$(PEAS_DLL_NAME).%%x del /f /q ..\peas-demo\$(PEAS_DLL_NAME).%%x
	@for %%x in (dll pdb) do @if exist ..\peas-demo\$(PEAS_GTK_DLL_NAME).%%x del /f /q ..\peas-demo\$(PEAS_GTK_DLL_NAME).%%x
	@for %%x in (exe pdb) do @if exist ..\peas-demo\peas-demo.%%x del /f /q ..\peas-demo\peas-demo.%%x
	@if exist $(CFG)\$(PLAT)\Introspection-1.0.typelib del $(CFG)\$(PLAT)\Introspection-1.0.typelib
	@if exist $(CFG)\$(PLAT)\Introspection-1.0.gir del $(CFG)\$(PLAT)\Introspection-1.0.gir
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
	@if exist $(CFG)\$(PLAT)\peas-test-introspection\ del /f /q $(CFG)\$(PLAT)\peas-test-introspection\*.obj
	@if exist $(CFG)\$(PLAT)\peas-test\ del /f /q $(CFG)\$(PLAT)\peas-test\*.obj
	@for %%x in (c h) do @(if exist $(CFG)\$(PLAT)\peas-test\embedded-resources.%%x del /f /q $(CFG)\$(PLAT)\peas-test\embedded-resources.%%x)
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
	@if exist $(CFG)\$(PLAT)\peas-test-introspection\ rmdir /s /q $(CFG)\$(PLAT)\peas-test-introspection
	@if exist $(CFG)\$(PLAT)\peas-test\ rmdir /s /q $(CFG)\$(PLAT)\peas-test
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

copy-loaders:
	@for %%x in (dll pdb) do @if exist $(CFG)\$(PLAT)\libpythonloader.%%x copy /b /y $(CFG)\$(PLAT)\libpythonloader.%%x ..\loaders\python
	@for %%x in (dll pdb) do @if exist $(CFG)\$(PLAT)\libpython3loader.%%x copy /b /y $(CFG)\$(PLAT)\libpython3loader.%%x ..\loaders\python3
	@for %%x in (dll pdb) do @if exist $(CFG)\$(PLAT)\liblua51loader.%%x copy /b /y $(CFG)\$(PLAT)\liblua51loader.%%x ..\loaders\lua5.1

tests: $(TEST_PROGS) copy-loaders
	@copy /b $(CFG)\$(PLAT)\Peas-$(APIVERSION).typelib ..\libpeas
	@if exist $(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).typelib copy /b $(CFG)\$(PLAT)\PeasGtk-$(APIVERSION).typelib ..\libpeas-gtk
	@copy /b $(CFG)\$(PLAT)\Introspection-1.0.typelib ..\tests\libpeas\introspection
	@for %%d in (builtin has-dep loadable self-dep) do @(for %%x in (dll pdb) do @copy /b $(CFG)\$(PLAT)\lib%%d.%%x ..\tests\plugins\%%d\)
	@for %%d in (embedded extension-c) do @(for %%x in (dll pdb) do @copy /b $(CFG)\$(PLAT)\lib%d.%x ..\tests\libpeas\plugins\%d\)
	@for %%x in (dll pdb) do @if exist $(CFG)\$(PLAT)\libextension-c-missing-symbol.%%x copy $(CFG)\$(PLAT)\libextension-c-missing-symbol.%%x ..\tests\libpeas\plugins\extension-c
	@for %%d in (builtin-configurable configurable) do @(for %%x in (dll pdb) do @copy /b $(CFG)\$(PLAT)\lib%%d.%%x ..\tests\libpeas-gtk\plugins\%%d\)
	@for %%x in (gschema.xml plugin) do @if "$(BUILD_PY2_LOADER)" == "1" $(PYTHON) replace.py -a=replace-str -i=..\tests\libpeas\plugins\extension-python\extension-py.%%x.in -o=..\tests\libpeas\plugins\extension-python\extension-python.%%x --instring=PY_LOADER --outstring=python
	@for %%x in (gschema.xml plugin) do @if "$(BUILD_PY3_LOADER)" == "1" $(PYTHON) replace.py -a=replace-str -i=..\tests\libpeas\plugins\extension-python\extension-py.%%x.in -o=..\tests\libpeas\plugins\extension-python\extension-python3.%%x --instring=PY_LOADER --outstring=python3
	@if "$(BUILD_PY2_LOADER)" == "1" copy ..\tests\libpeas\plugins\extension-python\extension-py.py.in ..\tests\libpeas\plugins\extension-python\extension-python.py
	@if "$(BUILD_PY3_LOADER)" == "1" copy ..\tests\libpeas\plugins\extension-python\extension-py.py.in ..\tests\libpeas\plugins\extension-python\extension-python3.py

prep-run-demo: copy-loaders
	@if not exist $(CFG)\$(PLAT)\peas-demo.exe (echo peas-demo is not built yet!) & (goto :eof)
	@for %%x in (exe pdb) do @copy /b /y $(CFG)\$(PLAT)\peas-demo.%%x ..\peas-demo
	@for %%x in (dll pdb) do @copy /b /y $(CFG)\$(PLAT)\$(PEAS_GTK_DLL_NAME).%%x ..\peas-demo
	@for %%x in (dll pdb) do @copy /b /y $(CFG)\$(PLAT)\$(PEAS_DLL_NAME).%%x ..\peas-demo
	@for %%x in (dll pdb) do @copy /b /y $(CFG)\$(PLAT)\libhelloworld.%%x ..\peas-demo\plugins\helloworld
	@for %%x in (dll pdb) do @copy /b /y $(CFG)\$(PLAT)\libsecondtime.%%x ..\peas-demo\plugins\secondtime
	@echo Please set the environment variables as follows to run the peas-demo program
	@echo directly from the build:
	@echo set GI_TYPELIB_PATH=%%CD%%\$(CFG)\$(PLAT)
	@echo Ensure that the lgi files can be loaded by your Lua installation
	@echo Go to ..\peas-demo and run "peas-demo.exe --run-from-build-dir"