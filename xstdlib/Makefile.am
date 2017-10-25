AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include

lib_LTLIBRARIES = libxstdlib.la
libxstdlib_la_SOURCES = src/xstdlib.cpp
libxstdlib_la_LDFLAGS = -version-info 0:0:0

include_HEADERS = include/xstdlib.h

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = xstdlib-${API_VERSION}.pc

