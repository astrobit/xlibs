AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include

lib_LIBRARIES = libxtime.a
libxtime_a_SOURCES = src/xtime.cpp
include_HEADERS = include/xtime.h

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = xtime-${API_VERSION}.pc

