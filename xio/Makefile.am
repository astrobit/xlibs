AUTOMAKE_OPTIONS = foreign subdir-objects


lib_LTLIBRARIES = libxio.la
libxio_la_SOURCES = src/xdataparse.cpp src/xdataset_advanced.cpp src/xdataset.cpp src/xdataset_improved.cpp src/xiocmn.cpp src/xmap.cpp
libxio_la_LDFLAGS = -version-info 1:0:1

#if HAVE_MPI
#lib_LTLIBRARIES += libxio_mpi.la
#libxio_mpi_la_SOURCES = src/xdataparse.cpp src/xdataset_advanced.cpp src/xdataset.cpp src/xdataset_improved.cpp src/xiocmn.cpp src/xmap.cpp
#libxio_mpi_la_LDFLAGS = -version-info 0:0:0
#endif
AM_CPPFLAGS = -I${srcdir}/include -I${top_srcdir}/xstdlib/include

include_HEADERS = include/xio.h

#src/%.st.o: src/%.cpp include/xio.h
#	$(COMPILE) -c $(patsubst %.st.o,%.cpp,$@) -o $@
#mpicxx:
#	CXX=$(MPICXX)

#.mpi.lo: mpicxx .cpp.lo

#clean-local:
#	-rm -f src/*.mpi.lo

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = xio-${API_VERSION}.pc #xio_mpi-${API_VERSION}.pc


