AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include

lib_LIBRARIES = libxstdlib.a
libxstdlib_a_SOURCES = src/xstdlib.cpp
include_HEADERS = include/xstdlib.h

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = xstdlib-${API_VERSION}.pc

