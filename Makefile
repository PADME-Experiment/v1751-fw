CC=g++
ROOTCFLAGS=$(shell root-config --cflags)
ROOTCLIBS=$(shell root-config --libs)
CFLAGS=-O2 -Wall

.PHONY: all clean build debug
.IGNORE: clean
.DEFAULT_GOAL:=all


debug: CFLAGS:=-g -O0 -Wall
debug: clean
debug: all


OBJFILES=$(patsubst %.cc,%.o,$(wildcard *.cc))
EXEC=analyser

all: $(EXEC)

$(EXEC): $(OBJFILES)
	$(CC) $(ROOTCLIBS) $(OBJFILES) -o $@

$(OBJFILES):%.o : %.cc  %.h
	$(CC) $(CFLAGS) $(ROOTCFLAGS) -c $< -o $@

clean:
	rm $(OBJFILES) $(EXEC)
