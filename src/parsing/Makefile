CC      = gcc
LIBS	= -I../utils -I../instruction -I../encode -I.
CFLAGS  = $(LIBS) -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -Ofast -c 

.SUFFIXES: .c .o

.PHONY: all clean

all: parse.a



parse.a: parser.o token_stream.o token.o 
	ar rcs ../$@ $^

parser.o: parser.c
	$(CC) $(CFLAGS) parser.c -c -o parser.o

token_stream.o: token_stream.c
	$(CC) $(CFLAGS) token_stream.c -c -o token_stream.o

token.o: token.c
	$(CC) $(CFLAGS) token.c -c -o token.o


clean:
	rm -f $(wildcard *.o)
	rm -f parse.a