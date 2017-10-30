AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = xrandtest xdatasettest flashtest mathpar xexpdbltest

AM_CPPFLAGS = -I${srcdir}/../xstdlib/include

xrandtest_SOURCES = src/xrand_test.cpp
xdatasettest_SOURCES = src/xdataset_testing.cpp
flashtest_SOURCES = src/FLASH_1D_Test.cpp
#xastroiontest_SOURCES = src/xastroion_test.cpp
mathpar_SOURCES = src/xmath_test_parallel.cpp
xexpdbltest_SOURCES = src/xexpdouble_test.cpp


xrandtest_LDADD = ${top_srcdir}/xstdlib/libxstdlib.la -lrt
xdatasettest_LDADD = ${top_srcdir}/xio/libxio.la ${top_srcdir}/xstdlib/libxstdlib.la
flashtest_LDADD =   ${top_srcdir}/xflash/libxflash.la -lhdf5
#xastroiontest_LDADD = ../xstdlib/libxstdlib.a ../xastro/libxastro.a ../xio/libxio.a ../xmath/libxmath.a
mathpar_LDADD = ${top_srcdir}/xmath/libxmath.la
xexpdbltest_LDADD = ${top_srcdir}/xmath/libxmath.la ${top_srcdir}/xstdlib/libxstdlib.la

xrandtest_CPPFLAGS =  -I${top_srcdir}/xstdlib/include
xdatasettest_CPPFLAGS =  -I${top_srcdir}/xio/include  -I${top_srcdir}/xstdlib/include
#xastroiontest_CPPFLAGS =  -I../xio/include  -I../xstdlib/include   -I../xastro/include  -I../xmath/include
flashtest_CPPFLAGS =  -I${top_srcdir}/xflash/include
mathpar_CPPFLAGS = $(OPENMP_CXXFLAGS) -I${top_srcdir}/xmath/include
xexpdbltest_CPPFLAGS = -I${top_srcdir}/xstdlib/include -I${top_srcdir}/xmath/include

mathpar_LDFLAGS = $(OPENMP_CXXFLAGS)
