AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include

lib_LTLIBRARIES = libxcpp.la
libxcpp_la_SOURCES = src/xcpp_dictionary.cpp src/xcpp_tokenizer.cpp
libxcpp_la_LDFLAGS = -version-info 0:0:0

include_HEADERS = include/xcpp.h

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = xcpp-${API_VERSION}.pc

