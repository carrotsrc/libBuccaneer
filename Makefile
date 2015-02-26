CPP=g++
CFLAGS=-ggdb -I./ -std=c++11 -I../rackoonio -I../local/include
LDFLAGS= `pkg-config --libs alsa` -lpthread -L../local/lib -L../rackoonio/ -L. -lrackio `pkg-config --libs sndfile alsa` -lpthread -lresample
LIBCFLAGS=$(CFLAGS) -fPIC -shared
LIBLDFLAGS=-Wl,-soname,libbuccaneer.so $(LDFLAGS)

LIBSOURCES := $(shell find -name '*.cpp' ! -name '*entry.cpp' ! -not -path 'tests')
LIBOBJECTS=$(patsubst %.cpp, %.o, $(LIBSOURCES))

TESTSOURCES := $(shell find tests/ -name '*.cpp')
TESTOBJECTS=$(patsubst %.cpp, %, $(TESTSOURCES))

$(LIBOBJECTS): %.o: %.cpp
	$(CPP) $(LIBCFLAGS) -c $< -o $@

libbuccaneer.so: $(LIBOBJECTS)
	$(CPP) $(LIBCFLAGS) $(LIBLDFLAGS) $(LIBOBJECTS) -o libbuccaneer.so


all: 
	make libbuccaneer.so

entry.o: $(libbuccaneer) entry.cpp
	$(CPP) $(CFLAGS) -c entry.cpp -o entry.o
	
racktest: entry.o
	$(CPP) $(CFLAGS) -lbuccaneer $(LDFLAGS) entry.o -o racktest

$(TESTOBJECTS): %: %.cpp
	$(CPP) $(CFLAGS) -lbuccaneer $(LDFLAGS) $< -o $@_libtest

tests: $(TESTOBJECTS)
	@echo "Build tests"

clean:
	find . -name "*.o" -type f -delete
 
