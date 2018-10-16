# NMake Makefile to build Introspection Files for libpeas

!include detectenv-msvc.mak

!if "$(PEAS_NO_GTK)" == ""
CHECK_PACKAGE = gtk+-3.0
!else
CHECK_PACKAGE = gobject-2.0
!endif

!include introspection-msvc.mak

!if "$(BUILD_INTROSPECTION)" == "TRUE"

setgirbuildenv:
	@set PYTHONPATH=$(PREFIX)\lib\gobject-introspection
	@set PATH=$(PREFIX)\bin;$(PATH)
	@set PKG_CONFIG_PATH=$(PKG_CONFIG_PATH)
	@set LIB=$(PREFIX)\lib;$(LIB)

$(CFG)/$(PLAT)/Peas-$(APIVERSION).gir: $(peas_introspection_sources) $(PEAS_DLL)
	@-echo Generating $@...
	$(PYTHON) $(G_IR_SCANNER)	\
	--verbose -no-libtool	\
	--namespace=Peas	\
	--nsversion=1.0	\
		\
	--library=peas-1.0	\
		\
	--add-include-path=$(G_IR_INCLUDEDIR)	\
	--include=GObject-2.0 --include=GModule-2.0 --include=Gio-2.0 --include=GIRepository-2.0	\
	--pkg-export=libpeas-1.0	\
	--cflags-begin	\
	$(PEAS_CFLAGS)	\
	--cflags-end	\
	--c-include=libpeas/peas.h --warn-all	\
	$(peas_introspection_sources)	\
	-L $(CFG)\$(PLAT)	\
	-o $@

$(CFG)/$(PLAT)/PeasGtk-$(APIVERSION).gir: $(peas_introspection_sources) $(PEAS_DLL) $(CFG)/$(PLAT)/Peas-$(APIVERSION).gir
	@-echo Generating $@...
	$(PYTHON) $(G_IR_SCANNER)	\
	--verbose -no-libtool	\
	--namespace=PeasGtk	\
	--nsversion=1.0	\
		\
	--library=peas-gtk-1.0	--library=peas-1.0	\
		\
	--add-include-path=$(G_IR_INCLUDEDIR)	\
	--include=GObject-2.0 --include=Gtk-3.0	\
	--pkg-export=libpeas-gtk-1.0	\
	--cflags-begin	\
	$(PEAS_GTK_CFLAGS)	\
	--cflags-end	\
	--include-uninstalled=$(CFG)/$(PLAT)/Peas-$(APIVERSION).gir --warn-all	\
	$(peas_gtk_introspection_sources)	\
	-L $(CFG)\$(PLAT)	\
	-o $@

$(CFG)/$(PLAT)/Introspection-1.0.gir: $(peas_test_introspection_sources) $(CFG)/$(PLAT)/introspection.lib $(CFG)/$(PLAT)/Peas-$(APIVERSION).gir
	@-echo Generating $@...
	$(PYTHON) $(G_IR_SCANNER)	\
	--verbose -no-libtool	\
	--namespace=Introspection	\
	--nsversion=1.0	\
		\
	--library=introspection	--library=peas-1.0	\
		\
	--add-include-path=$(G_IR_INCLUDEDIR)	\
	--include=GObject-2.0	\
	--pkg-export=libpeas-1.0	\
	--cflags-begin	\
	$(PEAS_CFLAGS)	\
	--cflags-end	\
	--warn-all	\
	$(peas_test_introspection_sources)	\
	-L $(CFG)\$(PLAT)	\
	-o $@

$(CFG)/$(PLAT)/Peas-$(APIVERSION).typelib: $(CFG)/$(PLAT)/Peas-$(APIVERSION).gir
	@-echo Compiling $@...
	$(G_IR_COMPILER)	\
	--debug --verbose	\
	$**	\
	-o $@

$(CFG)/$(PLAT)/PeasGtk-$(APIVERSION).typelib: $(CFG)/$(PLAT)/PeasGtk-$(APIVERSION).gir $(CFG)/$(PLAT)/Peas-$(APIVERSION).typelib
	@-echo Compiling $@...
	$(G_IR_COMPILER)	\
	--includedir=$(CFG)/$(PLAT) --debug --verbose	\
	$**	\
	-o $@

$(CFG)/$(PLAT)/Introspection-1.0.typelib: $(CFG)/$(PLAT)/Introspection-1.0.gir $(CFG)/$(PLAT)/Peas-$(APIVERSION).typelib
	@-echo Compiling $@...
	$(G_IR_COMPILER)	\
	--includedir=$(CFG)/$(PLAT) --debug --verbose	\
	$**	\
	-o $@

!else
all:
	@-echo $(ERROR_MSG)
!endif
