AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = xdatasettocsv xdatasetget xgenwiki textcol

AM_CPPFLAGS = -I${srcdir}/include

xdatasettocsv_SOURCES = src/xdatasettocsv.cpp
xdatasetget_SOURCES = src/xdatasetget.cpp
xgenwiki_SOURCES = src/xgenwiki.cpp
textcol_SOURCES = src/textcolumize.cpp

xdatasettocsv_LDADD = ../xio/libxio.a ../xstdlib/libxstdlib.a
xdatasetget_LDADD = ../xio/libxio.a ../xstdlib/libxstdlib.a
xgenwiki_LDADD = ../xcpp/libxcpp.a

xdatasettocsv_CPPFLAGS = -I${srcdir}/../xio/include -I${srcdir}/../xstdlib/include
xdatasetget_CPPFLAGS = -I${srcdir}/../xio/include -I${srcdir}/../xstdlib/include
xgenwiki_CPPFLAGS = -I${srcdir}/../xcpp/include

