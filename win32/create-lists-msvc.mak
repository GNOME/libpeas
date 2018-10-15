# Convert the source listing to object (.obj) listing in
# another NMake Makefile module, include it, and clean it up.
# This is a "fact-of-life" regarding NMake Makefiles...
# This file does not need to be changed unless one is maintaining the NMake Makefiles

# For those wanting to add things here:
# To add a list, do the following:
# # $(description_of_list)
# if [call create-lists.bat header $(makefile_snippet_file) $(variable_name)]
# endif
#
# if [call create-lists.bat file $(makefile_snippet_file) $(file_name)]
# endif
#
# if [call create-lists.bat footer $(makefile_snippet_file)]
# endif
# ... (repeat the if [call ...] lines in the above order if needed)
# !include $(makefile_snippet_file)
#
# (add the following after checking the entries in $(makefile_snippet_file) is correct)
# (the batch script appends to $(makefile_snippet_file), you will need to clear the file unless the following line is added)
#!if [del /f /q $(makefile_snippet_file)]
#!endif

# In order to obtain the .obj filename that is needed for NMake Makefiles to build DLLs/static LIBs or EXEs, do the following
# instead when doing 'if [call create-lists.bat file $(makefile_snippet_file) $(file_name)]'
# (repeat if there are multiple $(srcext)'s in $(source_list), ignore any headers):
# !if [for %c in ($(source_list)) do @if "%~xc" == ".$(srcext)" @call create-lists.bat file $(makefile_snippet_file) $(intdir)\%~nc.obj]
#
# $(intdir)\%~nc.obj needs to correspond to the rules added in build-rules-msvc.mak
# %~xc gives the file extension of a given file, %c in this case, so if %c is a.cc, %~xc means .cc
# %~nc gives the file name of a given file without extension, %c in this case, so if %c is a.cc, %~nc means a

NULL=

# For libpeas
!if [call create-lists.bat header peas_objs.mak peas_OBJS]
!endif

!if [for %c in ($(PEAS_SRCS) $(PEAS_MARSHAL_SRCS)) do @if "%~xc" == ".c" @call create-lists.bat file peas_objs.mak ^$(CFG)\^$(PLAT)\peas\%~nc.obj]
!endif

!if [call create-lists.bat footer peas_objs.mak]
!endif

!if [call create-lists.bat header peas_objs.mak PEAS_MARSHAL_SRCS]
!endif

!if [for %c in ($(PEAS_MARSHAL_SRCS)) do @call create-lists.bat file peas_objs.mak ^$(CFG)\^$(PLAT)\peas\%c]
!endif

!if [call create-lists.bat footer peas_objs.mak]
!endif

!if [call create-lists.bat header peas_objs.mak peas_introspection_sources]
!endif

!if [for %c in ($(PEAS_PUBLIC_HDRS) $(PEAS_SRCS)) do @call create-lists.bat file peas_objs.mak ..\libpeas\%c]
!endif

!if [call create-lists.bat footer peas_objs.mak]
!endif

# For libpeas-gtk
!if [call create-lists.bat header peas_objs.mak peas_gtk_OBJS]
!endif

!if [for %c in ($(PEAS_GTK_SRCS)) do @if "%~xc" == ".c" @call create-lists.bat file peas_objs.mak ^$(CFG)\^$(PLAT)\peas-gtk\%~nc.obj]
!endif

!if [call create-lists.bat footer peas_objs.mak]
!endif

!if [call create-lists.bat header peas_objs.mak peas_gtk_introspection_sources]
!endif

!if [for %c in ($(PEAS_GTK_PUBLIC_HDRS) $(PEAS_GTK_SRCS)) do @call create-lists.bat file peas_objs.mak ..\libpeas-gtk\%c]
!endif

!if [call create-lists.bat footer peas_objs.mak]
!endif

# For Python Loaders
!if [call create-lists.bat header peas_objs.mak peas_py_OBJS]
!endif

!if [for %c in ($(PYTHON_LOADER_SOURCES) peas-python-resources.c) do @if "%~xc" == ".c" @call create-lists.bat file peas_objs.mak ^$(CFG)\^$(PLAT)\loaders-py\%~nc.obj]
!endif

!if [call create-lists.bat footer peas_objs.mak]
!endif

!if [call create-lists.bat header peas_objs.mak peas_py3_OBJS]
!endif

!if [for %c in ($(PYTHON_LOADER_SOURCES) peas-python3-resources.c) do @if "%~xc" == ".c" @call create-lists.bat file peas_objs.mak ^$(CFG)\^$(PLAT)\loaders-py3\%~nc.obj]
!endif

!if [call create-lists.bat footer peas_objs.mak]
!endif

!if [call create-lists.bat header peas_objs.mak peas_lua_OBJS]
!endif

!if [for %c in ($(lua51loader_sources) peas-lua-resources.c) do @if "%~xc" == ".c" @call create-lists.bat file peas_objs.mak ^$(CFG)\^$(PLAT)\loaders-lua\%~nc.obj]
!endif

!if [call create-lists.bat footer peas_objs.mak]
!endif

!if [call create-lists.bat header peas_objs.mak peas_demo_OBJS]
!endif

!if [for %c in ($(PEAS_DEMO_SOURCES)) do @if "%~xc" == ".c" @call create-lists.bat file peas_objs.mak ^$(CFG)\^$(PLAT)\peas-demo\%~nc.obj]
!endif

!if [call create-lists.bat footer peas_objs.mak]
!endif

!if [call create-lists.bat header peas_objs.mak peas_demo_helloworld_OBJS]
!endif

!if [for %c in ($(DEMO_HELLOWORLD_SOURCES)) do @if "%~xc" == ".c" @call create-lists.bat file peas_objs.mak ^$(CFG)\^$(PLAT)\peas-demo-helloworld\%~nc.obj]
!endif

!if [call create-lists.bat footer peas_objs.mak]
!endif

!if [call create-lists.bat header peas_objs.mak peas_demo_secondtime_OBJS]
!endif

!if [for %c in ($(DEMO_SECONDTIME_SOURCES)) do @if "%~xc" == ".c" @call create-lists.bat file peas_objs.mak ^$(CFG)\^$(PLAT)\peas-demo-secondtime\%~nc.obj]
!endif

!if [call create-lists.bat footer peas_objs.mak]
!endif

!include peas_objs.mak

!if [del /f /q peas_objs.mak]
!endif