CC       = gcc
CLIBS	 = -Iinstruction -Iutils -I.
ASSLIBS  = -Iencode -Iparsing 
EMLIBS	 = -Idecode -Iexecute 
CFLAGS   = $(CLIBS) -Wall -g -std=c99 -Werror -pedantic -Ofast
ASSFLAGS = $(CFLAGS) $(ASSLIBS)
EMFLAGS	 = $(CFLAGS) $(EMLIBS) 
TFLAGS	 = $(CFLAGS) $(EMLIBS) $(ASSLIBS)

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.c assembler.o libparse.a libencode.a libinstruction.a libutils.a 
	$(CC)  $^ -o assemble $(ASSFLAGS)

emulate: emulate.c emulator.o pipeline.o libdecode.a libinstruction.a libexecute.a libutils.a
	$(CC) $^ -o emulate  $(EMFLAGS)

libencode.a: $(wildcard ./encode/*.c) $(wildcard ./encode/*.h)
	make -C encode

libdecode.a: $(wildcard ./decode/*.c) $(wildcard ./decode/*.h)
	make -C decode

libinstruction.a: $(wildcard ./instruction/*.c) $(wildcard ./instruction/*.h)
	make -C instruction

libparse.a: $(wildcard ./parse/*.c) $(wildcard ./parse/*.h)
	make -C parsing

libutils.a: $(wildcard ./utils/*.c) $(wildcard ./utils/*.h)
	make -C utils

libexecute.a: $(wildcard ./execute/*.c) $(wildcard ./execute/*.h)
	make -C execute

ptest: ptest.c emulator.o assembler.o pipeline.o
	$(CC) $(TFLAGS) $^ -o $@

emulator.o: emulator.c
	$(CC) $(EMFLAGS) emulator.c -c -o emulator.o

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
