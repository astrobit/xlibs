AUTOMAKE_OPTIONS = foreign subdir-objects
lib_LIBRARIES = libxmath.a
AM_CPPFLAGS = -I${srcdir}/include -I${srcdir}/../xstdlib/include

libxmath_a_SOURCES = src/xbinmath.cpp src/xcomplex.cpp src/xexpdouble.cpp src/xfit.cpp src/xindex_vector.cpp src/xmath.cpp src/xmatrix.cpp src/xmatrix_ep.cpp src/xmatrix_long.cpp src/xmatrix_tri.cpp src/xpolynomial.cpp src/xroots.cpp src/xstat.cpp src/xtensor.cpp src/xvector.cpp src/xvector_ep.cpp src/xvector_long.cpp

libxmath_a_CXXFLAGS = $(OPENMP_CXXFLAGS)

include_HEADERS = include/xbinmath.h include/xcomplex.h include/xextprec.h include/xfit.h include/xlinalgep.h include/xlinalg.h include/xmath.h include/xpolynomial.h include/xroots.h include/xstat.h

