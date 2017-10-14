AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = xrandtest xdatasettest flashtest mathpar xexpdbltest

AM_CPPFLAGS = -I../xstdlib/include

xrandtest_SOURCES = src/xrand_test.cpp
xdatasettest_SOURCES = src/xdataset_testing.cpp
flashtest_SOURCES = src/FLASH_1D_Test.cpp
#xastroiontest_SOURCES = src/xastroion_test.cpp
mathpar_SOURCES = src/xmath_test_parallel.cpp
xexpdbltest_SOURCES = src/xexpdouble_test.cpp


xrandtest_LDADD = ../xstdlib/libxstdlib.a
xdatasettest_LDADD = ../xio/libxio.a ../xstdlib/libxstdlib.a
flashtest_LDADD =   ../xflash/libxflash.a -lhdf5
#xastroiontest_LDADD = ../xstdlib/libxstdlib.a ../xastro/libxastro.a ../xio/libxio.a ../xmath/libxmath.a
mathpar_LDADD = ../xmath/libxmath.a
xexpdbltest_LDADD = ../xmath/libxmath.a ../xstdlib/libxstdlib.a

xrandtest_CPPFLAGS =  -I../xstdlib/include
xdatasettest_CPPFLAGS =  -I../xio/include  -I../xstdlib/include
#xastroiontest_CPPFLAGS =  -I../xio/include  -I../xstdlib/include   -I../xastro/include  -I../xmath/include
flashtest_CPPFLAGS =  -I../xflash/include
mathpar_CPPFLAGS = $(OPENMP_CXXFLAGS) -I../xmath/include
xexpdbltest_CPPFLAGS = -I../xstdlib/include -I../xmath/include

mathpar_LDFLAGS = $(OPENMP_CXXFLAGS)
