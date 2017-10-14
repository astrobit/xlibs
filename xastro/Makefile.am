AUTOMAKE_OPTIONS = foreign subdir-objects

lib_LIBRARIES = libxastro.a
libxastro_a_SOURCES = src/XAstro.cpp src/xastro_dates.cpp src/xastroion.cpp src/xastroline.cpp

include_HEADERS = include/xastro.h include/xastroion.h include/xastroline.h

pkgdata_DATA = share/allens_pf_data.csv share/Ar.dat share/Ca.dat share/C.dat share/Co.dat share/Cr.dat share/Fe.dat share/H.dat share/He.dat share/ionization_data.dat share/ionization_data.old share/Irwin_pf_data.csv share/Mg.dat share/Mn.dat share/N.dat share/Ne.dat share/Ni.dat share/O.dat share/pf_data.dat share/README share/S.dat share/Si.dat share/Ti.dat share/V.dat

AM_CPPFLAGS = -I./include -I../xmath/include -I../xio/include -I../xstdlib/include -DDATADIR=\"$(pkgdatadir)\"

