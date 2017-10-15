AUTOMAKE_OPTIONS = foreign subdir-objects

lib_LIBRARIES = libxastro.a
libxastro_a_SOURCES = src/XAstro.cpp src/xastro_dates.cpp src/xastroion.cpp src/xastroline.cpp

include_HEADERS = include/xastro.h include/xastroion.h include/xastroline.h

dist_pkgdata_DATA = ${srcdir}/share/allens_pf_data.csv ${srcdir}/share/Ar.dat share/Ca.dat ${srcdir}/share/C.dat share/Co.dat ${srcdir}/share/Cr.dat ${srcdir}/share/Fe.dat ${srcdir}/share/H.dat ${srcdir}/share/He.dat ${srcdir}/share/ionization_data.dat ${srcdir}/share/ionization_data.old ${srcdir}/share/Irwin_pf_data.csv ${srcdir}/share/Mg.dat ${srcdir}/share/Mn.dat ${srcdir}/share/N.dat ${srcdir}/share/Ne.dat ${srcdir}/share/Ni.dat ${srcdir}/share/O.dat ${srcdir}/share/pf_data.dat ${srcdir}/share/README ${srcdir}/share/S.dat ${srcdir}/share/Si.dat ${srcdir}/share/Ti.dat ${srcdir}/share/V.dat

AM_CPPFLAGS = -I${srcdir}/include -I${srcdir}/../xmath/include -I${srcdir}/../xio/include -I${srcdir}/../xstdlib/include -DDATADIR=\"$(pkgdatadir)\"

