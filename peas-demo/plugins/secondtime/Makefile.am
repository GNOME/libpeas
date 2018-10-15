plugindir = $(libdir)/peas-demo/plugins/secondtime

AM_CPPFLAGS = \
	-I$(top_srcdir) 	\
	$(PEAS_CFLAGS)		\
	$(PEAS_GTK_CFLAGS)

plugin_LTLIBRARIES = libsecondtime.la

include demo-secondtime-srcs.mak

libsecondtime_la_SOURCES = $(DEMO_SECONDTIME_SOURCES)

libsecondtime_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)
libsecondtime_la_LIBADD  = \
	$(top_builddir)/libpeas/libpeas-1.0.la	\
	$(PEAS_LIBS) 				\
	$(PEAS_GTK_LIBS)

plugin_DATA = secondtime.plugin

EXTRA_DIST = $(plugin_DATA)
