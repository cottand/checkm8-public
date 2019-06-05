CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -Ofast -c

.SUFFIXES: .c .o

.PHONY: all clean

all: libutils.a



libutils.a: llist.o loader.o shift.o symbol_table.o symbol.h writer.o
	ar rcs ../$@ $^

loader.o: loader.c
	$(CC) $(CFLAGS) loader.c -c -o loader.o

shift.o: shift.c
	$(CC) $(CFLAGS) shift.c -c -o shift.o

writer.o: writer.c
	$(CC) $(CFLAGS) writer.c -c -o writer.o

symbol_table.o: symbol_table.c
	$(CC) $(CFLAGS) symbol_table.c -c -o symbol_table.o

llist.o: llist.c
	$(CC) $(CFLAGS) llist.c -c -o llist.o

clean:
	rm -f $(wildcard *.o)
	rm -f libutils.a