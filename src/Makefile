CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o


emulate: emulate.c emulator.o loader.o pipeline.o decode.o execute.o instruction.o
	$(CC) $(CFLAGS) emulate.c emulator.o loader.o pipeline.o decode.o execute.o instruction.o -o emulate

emulator.o: emulator.c
	$(CC) $(CFLAGS) emulator.c -c -o emulator.o

loader.o: loader.c
	$(CC) $(CFLAGS) loader.c -c -o loader.o

pipeline.o: pipeline.c
	$(CC) $(CFLAGS) pipeline.c -c -o pipeline.o

decode.o: decode.c
	$(CC) $(CFLAGS) decode.c -c -o decode.o

execute.o: execute.c
	$(CC) $(CFLAGS) execute.c -c -o execute.o

instruction.o: instruction.c
	$(CC) $(CFLAGS) instruction.c -c -o instruction.o


clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
