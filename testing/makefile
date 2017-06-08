SRCDIR := ./src
BINDIR := ../bin
LIBDIR := ../lib
INCDIR := ../include
INCLUDES := $(wildcard $(INCDIR)/*.hpp) $(wildcard $(INCDIR)/*.h)
CXXFLAGSLCL := $(CXXFLAGS) -I$(INCDIR)
xiodep := $(LIBDIR)/libxio.a $(INCDIR)/xio.h
xstdlibdep := $(LIBDIR)/libxstdlib.a $(INCDIR)/xstdlib.h
xflashdep := $(LIBDIR)/libxflash.a $(INCDIR)/xflash.h
xastrodep := $(LIBDIR)/libxastro.a $(INCDIR)/xastro.h $(INCDIR)/xastroion.h $(INCDIR)/xastroline.h
xmathdep := $(LIBDIR)/libxmath.a $(INCDIR)/xmath.h $(INCDIR)/xfit.h $(INCDIR)/xlinalg.h $(INCDIR)/xpolynomial.h $(INCDIR)/xroots.h $(INCDIR)/xstat.h


all: xrandtest xdatasettest flashtest mathpar xexpdbltest #xastroiontest

.PHONY: all

$(BINDIR)/xrandtest: $(SRCDIR)/xrand_test.cpp $(xstdlibdep)
	$(CXX) $(CXXFLAGSLCL) $(SRCDIR)/xrand_test.cpp -lxstdlib -o $(BINDIR)/xrandtest
xrandtest: $(BINDIR)/xrandtest

$(BINDIR)/xdatasettest: $(SRCDIR)/xdataset_testing.cpp $(xstdlibdep) $(xiodep)
	$(CXX) $(CXXFLAGSLCL) $(SRCDIR)/xdataset_testing.cpp -lxio -lxstdlib -o $(BINDIR)/xdatasettest
xdatasettest: $(BINDIR)/xdatasettest

$(BINDIR)/FLASH_1D_Test: $(SRCDIR)/FLASH_1D_Test.cpp $(xflashdep)
	$(CXX) $(CXXFLAGSLCL) $(SRCDIR)/FLASH_1D_Test.cpp -lxflash $(HDF5LIBPATH) -lhdf5 -o $(BINDIR)/FLASH_1D_Test
flashtest: $(BINDIR)/FLASH_1D_Test

$(BINDIR)/xastroiontest: $(SRCDIR)/xastroion_test.cpp $(xiodep) $(xstdlibdep)  $(xastrodep) $(xmathdep)
	$(CXX) $(CXXFLAGSLCL) -fopenmp $(SRCDIR)/xastroion_test.cpp -lxstdlib -lxastro -lxio -lxmath -o $(BINDIR)/xastroiontest
astroiontest: $(BINDIR)/xastroiontest

$(BINDIR)/xmathpar: $(SRCDIR)/xmath_test_parallel.cpp $(xmathdep)
	$(CXX) $(CXXFLAGSLCL) -fopenmp $(SRCDIR)/xmath_test_parallel.cpp -lxmath -o $(BINDIR)/xmathpar
mathpar: $(BINDIR)/xmathpar

$(BINDIR)/xexpdbltest: $(SRCDIR)/xexpdouble_test.cpp $(xmathdep)
	$(CXX) $(CXXFLAGSLCL) -fopenmp $(SRCDIR)/xexpdouble_test.cpp -lxmath -o $(BINDIR)/xexpdbltest
xexpdbltest: $(BINDIR)/xexpdbltest $(SRCDIR)/xexpdouble_test.cpp

clean:


