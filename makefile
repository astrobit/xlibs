
libs: astro math stdlib io time flash cpp

all: libs xtools testing

.PHONY: all

BINDIR=./bin
MPICXX?=mpicxx
export MPICXX

astro: 
	@if [ ! -d xastro/obj ]; then mkdir xastro/obj; fi
	$(MAKE) -C xastro BARENAME=xastro

io: 
	@if [ ! -d xio/obj ]; then mkdir xio/obj; fi
	$(MAKE) -C xio BARENAME=xio

stdlib: 
	@if [ ! -d xstdlib/obj ]; then mkdir xstdlib/obj; fi
	$(MAKE) -C xstdlib BARENAME=xstdlib

time: 
	@if [ ! -d xtime/obj ]; then mkdir xtime/obj; fi
	$(MAKE) -C xtime BARENAME=xtime

flash: 
	@if [ ! -d xflash/obj ]; then mkdir xflash/obj; fi
	$(MAKE) -C xflash BARENAME=xflash CXXFLAGS="-DNO_NCDF=1 -DNO_HDF4=1"

math: 
	@if [ ! -d xmath/obj ]; then mkdir xmath/obj; fi
	$(MAKE) -C xmath BARENAME=xmath

cpp: 
	@if [ ! -d xcpp/obj ]; then mkdir xcpp/obj; fi
	$(MAKE) -C xcpp BARENAME=xcpp

xtools:
	@if [ ! -d xtools/obj ]; then mkdir xtools/obj; fi
	$(MAKE) -C xtools

testing: testing/src/*.cpp libs
	$(MAKE) -C testing


$(BINDIR)/xexpdbltest: testing/src/xexpdouble_test.cpp include/xextprec.h xmath/src/xexpdouble.cpp
	$(CXX) $(CXXFLAGSLCL) -fopenmp testing/src/xexpdouble_test.cpp xmath/src/xexpdouble.cpp -o $(BINDIR)/xexpdbltest
xexpdbltest: $(BINDIR)/xexpdbltest

#$(BINDIR)/xastroiontest: $(OBJDIR)/xastroion_test.o $(OBJDIR)/xastroion.o $(OBJDIR)/xastro.o $(OBJDIR)/xastroline.o $(LIBDIR)/libxstdlib.a astroiontest: $(BINDIR)/xastroiontest

clean:
	-rm $(BINDIR)/*
	$(MAKE) clean -C xastro BARENAME=xastro
	$(MAKE) clean -C xio BARENAME=xio
	$(MAKE) clean -C xstdlib BARENAME=xstdlib
	$(MAKE) clean -C xtime BARENAME=xtime
	$(MAKE) clean -C xflash BARENAME=xflash
	$(MAKE) clean -C xmath BARENAME=xmath
	$(MAKE) clean -C xcpp BARENAME=xcpp




