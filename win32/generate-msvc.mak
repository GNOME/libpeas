# NMake Makefile portion for code generation and
# intermediate build directory creation
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.

# Copy config.h
config.h: config.h.win32
	@copy $(@F).win32 $@

# Generate the marshalling sources
$(CFG)\$(PLAT)\peas\peas-marshal.h: ..\libpeas\peas-marshal.list
	$(GLIB_GENMARSHAL_CMD) ..\libpeas\$(*B).list	\
	--header --prefix=peas_cclosure_marshal > $@

$(CFG)\$(PLAT)\peas\peas-marshal.c: ..\libpeas\peas-marshal.list
	$(GLIB_GENMARSHAL_CMD) ..\libpeas\$(*B).list	\
	--body --prefix=peas_cclosure_marshal > $@.tmp
	
	@echo #include "$(@B).h" > $@
	@type $@.tmp >> $@
	@del $@.tmp

$(CFG)\$(PLAT)\loaders-py3\peas-python3-resources.c:	\
..\loaders\python3\peas-python3.gresource.xml
	$(PREFIX)\bin\glib-compile-resources.exe	\
	--target=$@			\
	--sourcedir=$(**D)	\
	--generate-source	\
	--internal			\
	$**

$(CFG)\$(PLAT)\loaders-py\peas-python-resources.c:	\
..\loaders\python\peas-python.gresource.xml
	$(PREFIX)\bin\glib-compile-resources.exe	\
	--target=$@			\
	--sourcedir=$(**D)	\
	--generate-source	\
	--internal			\
	$**

$(CFG)\$(PLAT)\loaders-lua\peas-lua-resources.c:	\
..\loaders\lua5.1\resources\peas-lua.gresource.xml
	$(PREFIX)\bin\glib-compile-resources.exe	\
	--target=$@			\
	--sourcedir=$(**D)	\
	--generate-source	\
	--internal			\
	$**

# Create the build directories

$(CFG)\$(PLAT)\peas	\
$(CFG)\$(PLAT)\peas-gtk	\
$(CFG)\$(PLAT)\loaders-py	\
$(CFG)\$(PLAT)\loaders-py3	\
$(CFG)\$(PLAT)\loaders-lua	\
$(CFG)\$(PLAT)\peas-demo	\
$(CFG)\$(PLAT)\peas-demo-helloworld	\
$(CFG)\$(PLAT)\peas-demo-secondtime:
	@-mkdir $@