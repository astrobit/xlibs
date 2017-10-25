AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include

lib_LTLIBRARIES = libxflash.la
libxflash_la_SOURCES = src/xflash_reader.cpp src/xflash_reader_hdf5.cpp src/xflash_reader_chombo.cpp 
libxflash_la_LDFLAGS = -version-info 0:0:0

include_HEADERS = include/xflash.h

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = xflash-${API_VERSION}.pc

