
TARGETS = mtp-sync
all: mtp-sync

SOURCES = main.cc mtpdevice.cc

OBJECTS := $(patsubst %.cc,%.o, $(SOURCES))

CXXFLAGS = -Wall -O2 -g -I.

%.o: %.cc headers.hh
	g++ $(CXXFLAGS) -c -std=c++11 -o $@ $<

clean:
	-rm -f $(TARGETS) $(OBJECTS)

LIBS = -lmtp

mtp-sync: $(OBJECTS)
	g++ -o $@ $(OBJECTS) $(LIBS) 
