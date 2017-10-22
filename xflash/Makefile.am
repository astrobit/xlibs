AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include

lib_LIBRARIES = libxflash.a
libxflash_a_SOURCES = src/xflash_reader.cpp src/xflash_reader_hdf5.cpp src/xflash_reader_chombo.cpp 

include_HEADERS = include/xflash.h

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = xflash-${API_VERSION}.pc

