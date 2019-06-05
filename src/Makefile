CC       = gcc
CLIBS	 = -Iinstruction -Iutils -L. -linstruction -lutils
ASSLIBS  = -Iencode -Iparsing -lencode -lparse
EMLIBS	 = -Idecode -Iexecute -ldecode -lexecute
CFLAGS   = $(CLIBS) -Wall -g -std=c99 -Werror -pedantic -Ofast
ASSFLAGS = $(CFLAGS) $(ASSLIBS)
EMFLAGS	 = $(CFLAGS) $(EMLIBS) 
TFLAGS	 = $(CFLAGS) $(EMLIBS) $(ASSLIBS)

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.c assembler.o libencode.a libinstruction.a libparsing.a libutils.a
	$(CC)  assembler.o assemble.c -o assemble $(ASSFLAGS)

emulate: emulate.c emulator.o pipeline.o libdecode.a libinstruction.a libexecute.a libutils.a
	$(CC) $(EMFLAGS) pipeline.o emulate.c emulator.o -o emulate

libencode.a:
	make -C encode

libdecode.a:
	make -C decode

libinstruction.a:
	make -C instruction

libparsing.a:
	make -C parsing

libutils.a:
	make -C utils

ptest: ptest.c emulator.o assembler.o pipeline.o
	$(CC) $(TFLAGS) $^ -o $@

emulator.o: emulator.c
	$(CC) $(EMLAGS) emulator.c -c -o emulator.o

pipeline.o: pipeline.c
	$(CC) $(EMFLAGS) pipeline.c -c -o pipeline.o

assembler.o: assembler.c
	$(CC) $(ASSFLAGS) assembler.c -c -o assembler.o

clean:
	rm -f $(wildcard *.o)
	rm -f $(wildcard *.a)
	rm -f assemble
	rm -f emulate
	rm -f ptest
	cd decode;	make clean
	cd encode;	make clean
	cd execute;	make clean
	cd instruction;	make clean
	cd parsing;	make clean
	cd utils;	make clean
