AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include

lib_LTLIBRARIES = libxtime.la
libxtime_la_SOURCES = src/xtime.cpp
libxtime_la_LDFLAGS = -version-info 0:0:0

include_HEADERS = include/xtime.h

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = xtime-${API_VERSION}.pc

