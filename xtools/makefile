SRCDIR := ./src
BINDIR := ../bin
LIBDIR := ../lib
INCDIR := ../include
INCLUDES := $(wildcard $(INCDIR)/*.hpp) $(wildcard $(INCDIR)/*.h)
CXXFLAGSLCL := $(CXXFLAGS) -I$(INCDIR)
XIODEP := $(LIBDIR)/libxio.a $(INCDIR)/xio.h
XSTDLIBDEP := $(LIBDIR)/libxstdlib.a $(INCDIR)/xstdlib.h

all: xdatasettocsv xdatasetget


$(BINDIR)/xdatasettocsv: $(SRCDIR)/xdatasettocsv.cpp $(XSTDLIBDEP) $(XIODEP)
	$(CXX) $(CXXFLAGSLCL) $(SRCDIR)/xdatasettocsv.cpp -lxio -lxstdlib -o $(BINDIR)/xdatasettocsv
xdatasettocsv: $(BINDIR)/xdatasettocsv

$(BINDIR)/xdatasetget: $(SRCDIR)/xdatasetget.cpp $(XSTDLIBDEP) $(XIODEP)
	$(CXX) $(CXXFLAGSLCL) $(SRCDIR)/xdatasetget.cpp -lxio -lxstdlib -o $(BINDIR)/xdatasetget
xdatasetget: $(BINDIR)/xdatasetget
 
