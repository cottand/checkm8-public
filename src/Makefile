CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o
	$(CC) $(CFLAGS) emulate.c emulator.c loader.c pipeline.c decoded_instr.c -I .

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
