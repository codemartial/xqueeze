# C Preprocessor flags
CPPFLAGS = 
# Linker flags
LDFLAGS = -L . -l$(LIBRARY)
# C compiler
CC = gcc
#C Compiler flags
CFLAGS = -Wall -O3 -pipe -fno-rtti -fPIC -I ..
#CFLAGS = -Wall -pipe -I..
# C++ Compiler
CXX = g++
# C++ Compiler Flags
CXXFLAGS = $(CFLAGS)
# Which linkage to use
LINKCC = $(CXX)

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
DEPS = $(patsubst %.cpp,%.d,$(wildcard *.cpp))

all: $(OBJS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.d: %.cpp
	$(CXX) -MM -I .. $(CPPFLAGS) $< > $@

clean:
	-rm $(DEPS) *~ *.o

depend: $(DEPS)
	@echo "Dependencies are now up-to-date."

-include $(DEPS)
