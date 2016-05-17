
libs: astro math stdlib io time flash

all: libs xtools testing

.PHONY: all

BINDIR=./bin
MPICXX?=mpicxx
export MPICXX

astro: 
	@echo $(MPICXX)
	$(MAKE) -C xastro BARENAME=xastro

io: 
	$(MAKE) -C xio BARENAME=xio

stdlib: 
	$(MAKE) -C xstdlib BARENAME=xstdlib

time: 
	$(MAKE) -C xtime BARENAME=xtime

flash: 
	$(MAKE) -C xflash BARENAME=xflash CXXFLAGS="-DNO_NCDF=1 -DNO_HDF4=1"

math: 
	$(MAKE) -C xmath BARENAME=xmath

xtools: 
	$(MAKE) -C xtools

testing:
	$(MAKE) -C testing



#$(BINDIR)/xastroiontest: $(OBJDIR)/xastroion_test.o $(OBJDIR)/xastroion.o $(OBJDIR)/xastro.o $(OBJDIR)/xastroline.o $(LIBDIR)/libxstdlib.a astroiontest: $(BINDIR)/xastroiontest

clean:
	-rm $(BINDIR)/*
	$(MAKE) clean -C xastro BARENAME=xastro
	$(MAKE) clean -C xio BARENAME=xio
	$(MAKE) clean -C xstdlib BARENAME=xstdlib
	$(MAKE) clean -C xtime BARENAME=xtime
	$(MAKE) clean -C xflash BARENAME=xflash
	$(MAKE) clean -C xmath BARENAME=xmath




