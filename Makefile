CPP=g++
CFLAGS=-ggdb -I./ -std=c++11 -I../rackoonio -I../local/include
LDFLAGS= `pkg-config --libs alsa` -lpthread -L../local/lib/ -L. -lrackio `pkg-config --libs sndfile alsa` -lpthread -lresample
LIBCFLAGS=$(CFLAGS) -fPIC -shared
LIBLDFLAGS=-Wl,-soname,libbuccaneer.so $(LDFLAGS)

LIBSOURCES := $(shell find -name '*.cpp' ! -name '*entry.cpp')
LIBOBJECTS=$(patsubst %.cpp, %.o, $(LIBSOURCES))


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
	
	

clean:
	find . -name "*.o" -type f -delete
 
