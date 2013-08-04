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
DIRS = common ContentHandler XMLReader xqAGenerator
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
HEADERS = $(wildcard xqml/*.hpp)
########################################

all: library

library: subdirs
	$(LINKCC) $(CXXFLAGS) -Wl,-soname -Wl,lib$(LIBRARY).so.$(MVER) -o lib$(LIBRARY).so.$(FULLVER) */*.o
	ln -sf lib$(LIBRARY).so.$(FULLVER) lib$(LIBRARY).so.$(MVER)
	ln -sf lib$(LIBRARY).so.$(MVER) lib$(LIBRARY).so

subdirs:
	ln -sf headers xqml
	@for dir in $(DIRS); do $(MAKE) -C $$dir; done

install:
	-mkdir $(INCLUDEDIR)/xqml
	cp headers/*.hpp /usr/include/xqml
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
	-rm -rf ../docs objects/ lib$(LIBRARY).so*
	@for dir in $(DIRS); do $(MAKE) -C $$dir clean; done
	$(MAKE) -C unittest clean
