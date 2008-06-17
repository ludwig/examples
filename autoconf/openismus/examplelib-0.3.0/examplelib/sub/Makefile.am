## sub directory

h_sources = sub.h
cc_sources = sub.cc

#Install the headers in a versioned directory - e.g. examplelib-1.0:
library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)-$(GENERIC_API_VERSION)/$(GENERIC_LIBRARY_NAME)/sub
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)

noinst_LTLIBRARIES= libsub.la
libsub_la_SOURCES= $(h_sources) $(cc_sources) 
