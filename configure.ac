AC_INIT(xlibs,1.0,astrobit2@yahoo.com,xlibs,https://github.com/astrobit/xlibs)

AM_INIT_AUTOMAKE

AC_PROG_RANLIB

AC_LANG(C++)

AC_PROG_CXX

AX_CXX_COMPILE_STDCXX(14,noext,mandatory) #use c++14 standard, required (I think) in xio and xstdlib, maybe xmath

AC_OPENMP #xmath requires openmp

AX_MPI([AM_CONDITIONAL([HAVE_MPI], [test "1" = "1"])],[AM_CONDITIONAL([HAVE_MPI], [test "1" = "2"])]) #MPI optional for xio


m4_ifdef([AM_PROG_AR], [AM_PROG_AR]) #not sure why the ifdef is required; this is supposed to be a standard command in automake, but my implementation (v1.15) seems to not find it.

AC_CHECK_HEADERS(hdf5.h) # make sure HDF5 is installed - 
#AC_CHECK_LIB(hdf5,H5Library) # I removed this one because the check_lib function appears to try linking only to a function of type char <fname>(void)

AC_CONFIG_FILES([Makefile xstdlib/Makefile xastro/Makefile xcpp/Makefile xflash/Makefile xio/Makefile xmath/Makefile xtime/Makefile testing/Makefile xtools/Makefile])


AC_OUTPUT


