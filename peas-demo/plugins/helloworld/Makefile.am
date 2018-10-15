plugindir = $(libdir)/peas-demo/plugins/helloworld

AM_CPPFLAGS = \
	-I$(top_srcdir) 	\
	$(PEAS_CFLAGS)		\
	$(PEAS_GTK_CFLAGS)

plugin_LTLIBRARIES = libhelloworld.la

include demo-helloworld-srcs.mak

libhelloworld_la_SOURCES = $(DEMO_HELLOWORLD_SOURCES)

libhelloworld_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)
libhelloworld_la_LIBADD  = \
	$(top_builddir)/libpeas-gtk/libpeas-gtk-1.0.la	\
	$(top_builddir)/libpeas/libpeas-1.0.la		\
	$(PEAS_LIBS)					\
	$(PEAS_GTK_LIBS)

plugin_DATA = helloworld.plugin

EXTRA_DIST = $(plugin_DATA)
