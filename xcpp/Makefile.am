AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include

lib_LIBRARIES = libxcpp.a
libxcpp_a_SOURCES = src/xcpp_dictionary.cpp src/xcpp_tokenizer.cpp

include_HEADERS = include/xcpp.h

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = xcpp-${API_VERSION}.pc

