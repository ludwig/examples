
dnl --------------------------------
dnl Initialization macros.
dnl --------------------------------

AC_INIT(src/main.cc)
AM_CONFIG_HEADER(config.h)

dnl -----------------------------------------------
dnl Package name and version number (user defined)
dnl -----------------------------------------------

AM_INIT_AUTOMAKE(examplelib_example, 0.3.0)


dnl -----------------------------------------------
dnl Checks for programs.
dnl -----------------------------------------------

AC_PROG_CC
AC_PROG_CXX
AM_PROG_LIBTOOL
AM_SANITY_CHECK

dnl -----------------------------------------------
dnl Checks for libraries.
dnl -----------------------------------------------

# Paths to examplelib stuff:
AC_LANG_CPLUSPLUS

PKG_CHECK_MODULES(DEPS, examplelib-1.0 >= 0.3.0)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)


dnl -----------------------------------------------
dnl Generates Makefiles.
dnl -----------------------------------------------

AC_OUTPUT(Makefile \
	  src/Makefile
)
