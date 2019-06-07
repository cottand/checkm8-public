CC      = gcc
LIBS	= -I../instruction
CFLAGS  = $(LIBS) -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -Ofast -c

.SUFFIXES: .c .o

.PHONY: all clean

all: libdecode.a


libdecode.a: decode.o
	ar rcs ../$@ $^

decode.o: decode.c decode.h
	$(CC) $(CFLAGS) decode.c -c -o decode.o

clean:
	rm -f $(wildcard *.o)
	rm -f libdecode.a