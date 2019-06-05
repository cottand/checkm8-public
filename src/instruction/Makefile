CC      = gcc
LIBS	= -I..
CFLAGS  = $(LIBS) -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -Ofast -c

.SUFFIXES: .c .o

.PHONY: all clean

all: libinstruction.a



libinstruction.a:
	$(CC) $(CFLAGS) instruction.c -o instruction.o
	ar rcs ../$@ instruction.o


clean:
	rm -f $(wildcard *.o)
	rm -f libinstruction.a