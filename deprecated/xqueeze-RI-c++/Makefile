# Top-level Makefile for Xqueeze-RI-c++

########################################
# Project Variables
########################################
#Library name
LIBRARY = xqueeze
# Major version number
MVER = 3
# Full version number
FULLVER = $(MVER).0.0
DIRS = common Parser Writer xqAGenerator
OBJDIR = objects
OBJS = $(wildcard */*.o)
########################################

########################################
#Compilation Options
########################################
LINKCC = $(CXX)
CPPFLAGS = 
CC = gcc
CFLAGS = 
CXX = g++
CXXFLAGS = -Wall -pipe -shared -I.. -fPIC
########################################

########################################
# Installation options
########################################
# Default directory for headers
INCLUDEDIR = /usr/include
# Default directory for shared library files
LIBDIR = /usr/lib
# Headers files to be installed
HEADERS = $(wildcard xqml/*.h)
########################################

all: subdirs library

subdirs:
	@for dir in $(DIRS); do $(MAKE) -C $$dir; done
library: $(OBJS)
	$(LINKCC) $(CXXFLAGS) -Wl,-soname -Wl,lib$(LIBRARY).so.$(MVER) -o lib$(LIBRARY).so.$(FULLVER) $(OBJS)
	ln -sf lib$(LIBRARY).so.$(FULLVER) lib$(LIBRARY).so.$(MVER)
	ln -sf lib$(LIBRARY).so.$(MVER) lib$(LIBRARY).so

install:
	-mkdir $(INCLUDEDIR)/xqml
	cp headers/*.h /usr/include/xqml
	cp lib$(LIBRARY).so.$(FULLVER) $(LIBDIR)
	cd $(LIBDIR) ; ln -sf lib$(LIBRARY).so.$(MVER) lib$(LIBRARY).so
	/sbin/ldconfig

uninstall:
	cd $(INCLUDEDIR); rm $(HEADERS)
	-rmdir $(INCLUDEDIR)/xqml
	rm $(LIBDIR)/libxqueeze.so*
	/sbin/ldconfig

docs: 
	doxygen Doxyfile
clean:
	-rm -rf SourceDocs lib$(LIBRARY).so*
	@for dir in $(DIRS); do $(MAKE) -C $$dir clean; done
	$(MAKE) -C test clean
