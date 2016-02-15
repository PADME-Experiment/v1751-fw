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
OBJFILES=main.o
OBJFILES+=2016-02-10-ledpmt-analyse_burst.o
OBJFILES+=2016-02-10-ledpmt-analyse_run.o
#OBJFILES+=analyse_burst.o
#OBJFILES+=analyse_run.o
OBJFILES+=caen_raw.o

EXEC=analyser

all: $(EXEC)

$(EXEC): $(OBJFILES)
	$(CC) $(ROOTCLIBS) $(OBJFILES) -o $@

$(OBJFILES):%.o : %.cc *.h  #%.h
	$(CC) $(CFLAGS) $(ROOTCFLAGS) -c $< -o $@

clean:
	rm $(OBJFILES) $(EXEC)
