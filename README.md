# xlibs
Set of common routines in c/c++ for a variety of purposes, including command line processing, string processing, data file processing, linear algebra, interpolation and extrapolation, integration, root finding, model fitting, complex arithmatic, random numbers, mathematical, physical, and astronomical constants, and some other miscellaneous routines.

All code in these libraries is covered under GPL.

As of 4 Sept 2015 the random number generators have been completed, but have not yet been tested.  There is an incomplete test utilitity (xrandtest).  That attempts to at least find the periodicity of the Park-Miller generator.  It needs to be extended to test the other generators, as well as perform a complete set of random number tests.

It is notable that the Park-Miller a and m values have been criticized as being a 'poor' generator, with a period and range < m.  All generators included here suffer from problems: specifically none will produce a true 0 as all of them use MLCG.  A future implementation should include LCGs such that true 0 can be achieved.

Use of code in these libraries is at the user's own risk.  No guaruntee of the safety or applicability of any code is made by the author.


Pulls of code prior to commit 5f9b606 is forbidden due to licensing.

prerequisites:
	autotools
	(optional) mpi (any form)
	openmp capable compiler (required for xmath)
	HDF5 (for xflash only)

build instructions:
	./configure
		use ./configure --prefix=[install path] to install somewhere other than /usr/...
	make
	make install

Installed files:
	header files:
		xastro:
			include/xastro.h
			include/xastroion.h
			include/xastroline.h
		xcpp:
			include/xcpp.h
		xflash:
			include/xflash.h
		xio:
			include/xio.h
		xmath:
			include/xbinmath.h
			include/xcomplex.h
			include/xextprec.h
			include/xfit.h
			include/xgraph.h
			include/xlinalgep.h
			include/xlinalg.h
			include/xmath.h
			include/xpolynomial.h
			include/xroots.h
			include/xstat.h
		xstdlib:
			include/xstdlib.h
		xtime:
			include/xtime.h
	libraries:
		lib/libxastro.a
		lib/libxcpp.a
		lib/libxflash.a
		lib/libxio.a
		lib/libxio-mpi.a
		lib/libxmath.a
		lib/libxstdlib.a
		lib/libxtime.a
	executables:
		bin/flashtest
		bin/mathpar
		bin/textcol
		bin/xdatasetget
		bin/xdatasettest
		bin/xdatasettocsv
		bin/xexpdbltest
		bin/xgenwiki
		bin/xrandtest
	other:
		Several data files supporting xastroion and xastroline in share/xlibs
		a modulefile in ~/privatemodules or in the shared data directory (share/xlibs/xlibs/<version>) if the package is not installed in /usr/local


