# Which linkage to use
LINKCC = $(CXX)
# C Preprocessor flags
CPPFLAGS = 
# Linker flags
LDFLAGS = -L . -l$(LIBRARY)
# C compiler
CC = gcc
#C Compiler flags
CFLAGS = -Wall -O3 -pipe -fPIC -I ..
# C++ Compiler
CXX = g++
# C++ Compiler Flags
CXXFLAGS = $(CFLAGS)

SRCS = $(wildcard *.C)
OBJS = $(patsubst %.C,%.o,$(wildcard *.C))
DEPS = $(patsubst %.C,%.d,$(wildcard *.C))

all: $(OBJS)

%.o: %.C
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.d: %.C
	$(CXX) -MM -I .. $(CPPFLAGS) $< > $@

clean:
	-rm $(DEPS) *~ *.o

depend: $(DEPS)
	@echo "Dependencies are now up-to-date."

-include $(DEPS)
