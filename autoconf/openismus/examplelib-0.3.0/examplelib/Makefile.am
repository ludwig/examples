## Source directory

SUBDIRS = sub

h_sources = examplelib.h something.h
cc_sources = something.cc

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)-$(GENERIC_API_VERSION)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)

lib_LTLIBRARIES= libexamplelib-1.0.la
libexamplelib_1_0_la_SOURCES= $(h_sources) $(cc_sources)
libexamplelib_1_0_la_LIBADD= sub/libsub.la
libexamplelib_1_0_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

