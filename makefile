all: astro math stdlib io time flash flashtest astroiontest xtools xrandtest xdatasettest xdatasetget

xtools: xdatasettocsv xdatasetget

.PHONY: all

LIBDIR=./lib
OBJDIR=./obj
BINDIR=./bin
INCLUDEDIR=./include
CCOMP=mpicxx
FLAGS= -D_isnan=isnan -D__max=_max -D__min=_min -D_finite=finite -DMPICH_IGNORE_CXX_SEEK -Wall -I$(INCLUDEDIR)
HDF5LIBPATH=

$(LIBDIR)/libxtime.a: $(OBJDIR)/xtime.o
	-rm $(LIBDIR)/libxtime.a
	ar -cvr $(LIBDIR)/libxtime.a $(OBJDIR)/xtime.o
time: $(LIBDIR)/libxtime.a

$(LIBDIR)/libxflash.a: $(OBJDIR)/xflash_reader.o $(OBJDIR)/xflash_reader_hdf5.o $(OBJDIR)/xflash_reader_chombo.o
	-rm $(LIBDIR)/libxflash.a
	ar -cvr $(LIBDIR)/libxflash.a $(OBJDIR)/xflash_reader.o $(OBJDIR)/xflash_reader_hdf5.o $(OBJDIR)/xflash_reader_chombo.o
flash: $(LIBDIR)/libxflash.a

$(BINDIR)/FLASH_1D_Test: $(OBJDIR)/xflash_reader.o $(OBJDIR)/xflash_reader_hdf5.o $(OBJDIR)/xflash_reader_chombo.o $(OBJDIR)/FLASH_1D_Test.o
	$(CCOMP) $(FLAGS) $(OBJDIR)/xflash_reader.o $(OBJDIR)/xflash_reader_hdf5.o $(OBJDIR)/xflash_reader_chombo.o $(OBJDIR)/FLASH_1D_Test.o $(HDF5LIBPATH) -lhdf5 -o $(BINDIR)/FLASH_1D_Test
flashtest: $(BINDIR)/FLASH_1D_Test

$(BINDIR)/xastroiontest: $(OBJDIR)/xastroion_test.o $(OBJDIR)/xastroion.o $(OBJDIR)/xastro.o $(OBJDIR)/xastroline.o $(LIBDIR)/libxstdlib.a $(LIBDIR)/libxastro.a $(LIBDIR)/libxio.a $(LIBDIR)/libxmath.a
	-rm $(BINDIR)/xastroiontest
	$(CCOMP) $(FLAGS)  $(OBJDIR)/xastroion_test.o -lxstdlib -lxastro -lxio -lxmath -o $(BINDIR)/xastroiontest
astroiontest: $(BINDIR)/xastroiontest

$(LIBDIR)/libxastro.a: $(OBJDIR)/xastro.o $(OBJDIR)/xastroline.o $(OBJDIR)/xastroion.o
	-rm $(LIBDIR)/libxastro.a
	ar -cvr $(LIBDIR)/libxastro.a  $(OBJDIR)/xastro.o $(OBJDIR)/xastroline.o $(OBJDIR)/xastroion.o
astro: $(LIBDIR)/libxastro.a

$(LIBDIR)/libxio.a: $(OBJDIR)/xdataset.o $(OBJDIR)/xiocmn.o $(OBJDIR)/xdataset_advanced.o $(OBJDIR)/xmap.o
	-rm $(LIBDIR)/libxio.a
	ar -cvr $(LIBDIR)/libxio.a $(OBJDIR)/xdataset.o $(OBJDIR)/xiocmn.o $(OBJDIR)/xdataset_advanced.o $(OBJDIR)/xmap.o
io: $(LIBDIR)/libxio.a

$(LIBDIR)/libxmath.a: $(OBJDIR)/xcomplex.o $(OBJDIR)/xfit.o $(OBJDIR)/xindex_vector.o $(OBJDIR)/xmath.o $(OBJDIR)/xmatrix.o $(OBJDIR)/xmatrix_tri.o $(OBJDIR)/xpolynomial.o $(OBJDIR)/xroots.o $(OBJDIR)/xstat.o $(OBJDIR)/xtensor.o $(OBJDIR)/xvector.o
	-rm $(LIBDIR)/libxmath.a
	ar -cvr $(LIBDIR)/libxmath.a $(OBJDIR)/xcomplex.o $(OBJDIR)/xfit.o $(OBJDIR)/xindex_vector.o $(OBJDIR)/xmath.o $(OBJDIR)/xmatrix.o $(OBJDIR)/xmatrix_tri.o $(OBJDIR)/xpolynomial.o $(OBJDIR)/xroots.o $(OBJDIR)/xstat.o $(OBJDIR)/xtensor.o $(OBJDIR)/xvector.o
math: $(LIBDIR)/libxmath.a

$(LIBDIR)/libxstdlib.a: $(OBJDIR)/xstdlib.o
	-rm $(LIBDIR)/libxstdlib.a
	ar -cvr $(LIBDIR)/libxstdlib.a $(OBJDIR)/xstdlib.o
stdlib: $(LIBDIR)/libxstdlib.a

$(OBJDIR)/xastro.o: xastro/XAstro.cpp include/xastro.h
	$(CCOMP) $(FLAGS) -c xastro/XAstro.cpp -o $(OBJDIR)/xastro.o

$(OBJDIR)/xastroline.o: xastro/xastroline.cpp include/xastroline.h
	$(CCOMP) $(FLAGS) -c xastro/xastroline.cpp -o $(OBJDIR)/xastroline.o

$(OBJDIR)/xastroion.o: xastro/xastroion.cpp include/xastroion.h
	$(CCOMP) $(FLAGS) -c xastro/xastroion.cpp -o $(OBJDIR)/xastroion.o

$(OBJDIR)/xmap.o: xio/xmap.cpp include/xio.h
	$(CCOMP) $(FLAGS) -c xio/xmap.cpp -o $(OBJDIR)/xmap.o

$(OBJDIR)/xdataset.o: xio/xdataset.cpp include/xio.h
	$(CCOMP) $(FLAGS) -c xio/xdataset.cpp -o $(OBJDIR)/xdataset.o

$(OBJDIR)/xdataset_advanced.o: xio/xdataset_advanced.cpp include/xio.h
	$(CCOMP) $(FLAGS) -c xio/xdataset_advanced.cpp -o $(OBJDIR)/xdataset_advanced.o

$(OBJDIR)/xiocmn.o: xio/xiocmn.cpp include/xio.h
	$(CCOMP) $(FLAGS) -c xio/xiocmn.cpp -o $(OBJDIR)/xiocmn.o

$(OBJDIR)/xcomplex.o: xmath/xcomplex.cpp
	$(CCOMP) $(FLAGS) -c xmath/xcomplex.cpp -o $(OBJDIR)/xcomplex.o

$(OBJDIR)/xfit.o: xmath/xfit.cpp include/xfit.h
	$(CCOMP) $(FLAGS) -c xmath/xfit.cpp -o $(OBJDIR)/xfit.o

$(OBJDIR)/xindex_vector.o: xmath/xindex_vector.cpp include/xlinalg.h
	$(CCOMP) $(FLAGS) -c xmath/xindex_vector.cpp -o $(OBJDIR)/xindex_vector.o

$(OBJDIR)/xmath.o: xmath/xmath.cpp include/xmath.h
	$(CCOMP) $(FLAGS) -c xmath/xmath.cpp -o $(OBJDIR)/xmath.o

$(OBJDIR)/xmatrix.o: xmath/xmatrix.cpp include/xlinalg.h
	$(CCOMP) $(FLAGS) -c xmath/xmatrix.cpp -o $(OBJDIR)/xmatrix.o

$(OBJDIR)/xmatrix_tri.o: xmath/xmatrix_tri.cpp include/xlinalg.h
	$(CCOMP) $(FLAGS) -c xmath/xmatrix_tri.cpp -o $(OBJDIR)/xmatrix_tri.o

$(OBJDIR)/xpolynomial.o: xmath/xpolynomial.cpp include/xpolynomial.h
	$(CCOMP) $(FLAGS) -c xmath/xpolynomial.cpp -o $(OBJDIR)/xpolynomial.o

$(OBJDIR)/xroots.o: xmath/xroots.cpp include/xmath.h
	$(CCOMP) $(FLAGS) -c xmath/xroots.cpp -o $(OBJDIR)/xroots.o

$(OBJDIR)/xstat.o: xmath/xstat.cpp include/xstat.h
	$(CCOMP) $(FLAGS) -c xmath/xstat.cpp -o $(OBJDIR)/xstat.o

$(OBJDIR)/xtensor.o: xmath/xtensor.cpp include/xlinalg.h
	$(CCOMP) $(FLAGS) -c xmath/xtensor.cpp -o $(OBJDIR)/xtensor.o

$(OBJDIR)/xvector.o: xmath/xvector.cpp include/xlinalg.h
	$(CCOMP) $(FLAGS) -c xmath/xvector.cpp -o $(OBJDIR)/xvector.o

$(OBJDIR)/xstdlib.o: xstdlib/xstdlib.cpp include/xstdlib.h
	$(CCOMP) $(FLAGS) -c xstdlib/xstdlib.cpp -o $(OBJDIR)/xstdlib.o

$(OBJDIR)/xtime.o: xtime/xtime.cpp include/xtime.h
	$(CCOMP) $(FLAGS) -c xtime/xtime.cpp -o $(OBJDIR)/xtime.o

$(OBJDIR)/xflash_reader.o: xflash/xflash_reader.cpp include/xflash.h
	$(CCOMP) $(FLAGS) -c xflash/xflash_reader.cpp -o $(OBJDIR)/xflash_reader.o

$(OBJDIR)/xflash_reader_hdf5.o: xflash/xflash_reader_hdf5.cpp include/xflash.h
	$(CCOMP) $(FLAGS) -c xflash/xflash_reader_hdf5.cpp -o $(OBJDIR)/xflash_reader_hdf5.o

$(OBJDIR)/xflash_reader_chombo.o: xflash/xflash_reader_chombo.cpp include/xflash.h
	$(CCOMP) $(FLAGS) -c xflash/xflash_reader_chombo.cpp -o $(OBJDIR)/xflash_reader_chombo.o

$(OBJDIR)/FLASH_1D_Test.o: testing/FLASH_1D_Test.cpp
	$(CCOMP) $(FLAGS) -c testing/FLASH_1D_Test.cpp -o $(OBJDIR)/FLASH_1D_Test.o

$(OBJDIR)/xastroion_test.o: testing/xastroion_test.cpp
	$(CCOMP) $(FLAGS) -c testing/xastroion_test.cpp -o $(OBJDIR)/xastroion_test.o

$(BINDIR)/xdatasettocsv: xtools/xdatasettocsv.cpp $(LIBDIR)/libxio.a $(LIBDIR)/libxstdlib.a
	$(CCOMP) $(FLAGS) xtools/xdatasettocsv.cpp -lxio -lxstdlib -o $(BINDIR)/xdatasettocsv
xdatasettocsv: $(BINDIR)/xdatasettocsv

$(BINDIR)/xrandtest: testing/xrand_test.cpp $(LIBDIR)/libxstdlib.a $(INCLUDEDIR)/xstdlib.h
	$(CCOMP) $(FLAGS) testing/xrand_test.cpp -lxstdlib -o $(BINDIR)/xrandtest
xrandtest: $(BINDIR)/xrandtest

$(BINDIR)/xdatasettest: testing/xdataset_testing.cpp $(LIBDIR)/libxio.a $(INCLUDEDIR)/xio.h $(LIBDIR)/libxstdlib.a $(INCLUDEDIR)/xstdlib.h
	$(CCOMP) $(FLAGS) testing/xdataset_testing.cpp -lxio -lxstdlib -o $(BINDIR)/xdatasettest
xdatasettest: $(BINDIR)/xdatasettest

$(BINDIR)/xdatasetget: xtools/xdatasetget.cpp $(LIBDIR)/libxio.a $(INCLUDEDIR)/xio.h $(LIBDIR)/libxstdlib.a $(INCLUDEDIR)/xstdlib.h
	$(CCOMP) $(FLAGS) xtools/xdatasetget.cpp -lxio -lxstdlib -o $(BINDIR)/xdatasetget
xdatasetget: $(BINDIR)/xdatasetget

clean:
	-rm $(LIBDIR)/*.a.a
	-rm FLASH_1D_Test
	-rm $(OBJDIR)/*
	-rm $(BINDIR)/*



