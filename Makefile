CC=g++
ROOTFLAGS=`root-config --cflags --libs`
CFLAGS=-O2
CLIBS=
#CFLAGS=-g -O0

CPPROOT=$(CC) $(CFLAGS) $(CLIBS) $(ROOTFLAGS)

.PHONY: all clean build
.IGNORE: clean
.DEFAULT_GOAL:=all


OBJFILES=analyse.o caen-raw.o
EXEC=analyser

all: $(EXEC)

$(EXEC): $(OBJFILES)
	$(CPPROOT) $(OBJFILES) -o $@

$(OBJFILES):%.o : %.cc  %.h
	$(CPPROOT) -c $< -o $@

clean:
	rm $(OBJFILES) $(EXEC)
