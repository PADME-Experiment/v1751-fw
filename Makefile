CC=g++
ROOTCFLAGS=$(shell root-config --cflags)
ROOTCLIBS=$(shell root-config --libs)
CFLAGS=-O2
#CFLAGS=-g -O0

.PHONY: all clean build
.IGNORE: clean
.DEFAULT_GOAL:=all


OBJFILES=$(patsubst %.cc,%.o,$(wildcard *.cc))
EXEC=analyser

all: $(EXEC)

$(EXEC): $(OBJFILES)
	$(CC) $(ROOTCLIBS) $(OBJFILES) -o $@

$(OBJFILES):%.o : %.cc  %.h
	$(CC) $(CFLAGS) $(ROOTCFLAGS) -c $< -o $@

clean:
	rm $(OBJFILES) $(EXEC)
