AUTOMAKE_OPTIONS = foreign subdir-objects


lib_LIBRARIES = libxio.a
libxio_a_SOURCES = src/xdataparse.cpp src/xdataset_advanced.cpp src/xdataset.cpp src/xdataset_improved.cpp src/xiocmn.cpp src/xmap.cpp

if HAVE_MPI
lib_LIBRARIES += libxio_mpi.a
libxio_mpi_a_SOURCES = 
libxio_mpi_a_LIBADD = src/xdataparse.mpi.o src/xdataset_advanced.mpi.o src/xdataset.mpi.o src/xdataset_improved.mpi.o src/xiocmn.mpi.o src/xmap.mpi.o
endif
AM_CPPFLAGS = -I${srcdir}/include -I${srcdir}/../xstdlib/include

include_HEADERS = include/xio.h

#src/%.st.o: src/%.cpp include/xio.h
#	$(COMPILE) -c $(patsubst %.st.o,%.cpp,$@) -o $@

src/%.mpi.o: ${srcdir}/src/%.cpp ${srcdir}/include/xio.h
	$(MPICXX)  $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -DPARALLEL -c $(patsubst %.mpi.o,$(srcdir)/%.cpp,$@) -o $@

clean-local:
	-rm -f src/*.mpi.o

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = xio-${API_VERSION}.pc xio_mpi-${API_VERSION}.pc


