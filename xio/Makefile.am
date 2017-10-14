AUTOMAKE_OPTIONS = foreign subdir-objects


lib_LIBRARIES = libxio.a
libxio_a_SOURCES = 
libxio_a_LIBADD = src/xdataparse.st.o src/xdataset_advanced.st.o src/xdataset.st.o src/xdataset_improved.st.o src/xiocmn.st.o src/xmap.st.o

if HAVE_MPI
lib_LIBRARIES += libxio_mpi.a
libxio_mpi_a_SOURCES = 
libxio_mpi_a_LIBADD = src/xdataparse.mpi.o src/xdataset_advanced.mpi.o src/xdataset.mpi.o src/xdataset_improved.mpi.o src/xiocmn.mpi.o src/xmap.mpi.o
endif
AM_CPPFLAGS = -I./include -I../xstdlib/include

src/%.st.o: src/%.cpp include/xio.h
	$(COMPILE) $(CXXFLAGS) $(AM_CPPFLAGS) -c $(patsubst %.st.o,%.cpp,$@) -o $@

src/%.mpi.o: src/%.cpp include/xio.h
	$(MPICXX) $(CXXFLAGS) $(AM_CPPFLAGS) -DPARALLEL -c $(patsubst %.mpi.o,%.cpp,$@) -o $@

clean-local:
	-rm -f *.mpi.o
	-rm -f *.st.o
