AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = xdatasettocsv xdatasetget xgenwiki textcol

AM_CPPFLAGS = -I${srcdir}/include

xdatasettocsv_SOURCES = src/xdatasettocsv.cpp
xdatasetget_SOURCES = src/xdatasetget.cpp
xgenwiki_SOURCES = src/xgenwiki.cpp
textcol_SOURCES = src/textcolumize.cpp

xdatasettocsv_LDADD = ${top_srcdir}/xio/libxio.la ${top_srcdir}/xstdlib/libxstdlib.la
xdatasetget_LDADD = ${top_srcdir}/xio/libxio.la ${top_srcdir}/xstdlib/libxstdlib.la
xgenwiki_LDADD = ${top_srcdir}/xcpp/libxcpp.la

xdatasettocsv_CPPFLAGS = -I${top_srcdir}xio/include -I${top_srcdir}/xstdlib/include
xdatasetget_CPPFLAGS = -I${top_srcdir}xio/include -I${top_srcdir}xstdlib/include
xgenwiki_CPPFLAGS = -I${top_srcdir}xcpp/include

