AUTOMAKE_OPTIONS = gnu
LDADD =    @GTK_LIBS@
CPPFLAGS = @GTK_CFLAGS@
bin_PROGRAMS = gdu
gdu_SOURCES = gdu.cc tree.cc

noinst_HEADERS = gdu.h

# man page
man_MANS = gdu.1x

# we want these in the dist tarball
EXTRA_DIST = back.xpm reload.xpm root.xpm \
             home.xpm stop.xpm about.xpm gdu.1x
