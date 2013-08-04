LINKCC = $(CXX)

LIBRARY = xqueeze

CPPFLAGS = 

CC = gcc
CFLAGS = -Wall -I.. -L..

CXX = g++
CXXFLAGS = $(CFLAGS)

all: xqagen_test writer_test parser_test

parser_test: parser_test.C
	$(CXX) $(CXXFLAGS) parser_test.C -l$(LIBRARY) -o parser_test

writer_test: writer_test.C 
	$(CXX) $(CXXFLAGS) writer_test.C -l$(LIBRARY) -o writer_test

xqagen_test: xqagen_test.C
	$(CXX) $(CXXFLAGS) xqagen_test.C -l$(LIBRARY) -o xqagen_test

clean:
	-rm *_test *~ test.x* test.dat

test: 
	./xqagen_test 2&> /dev/null
	./writer_test 2&> /dev/null 
	./parser_test
	./parser_test -t
