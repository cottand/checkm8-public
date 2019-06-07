CC      = gcc
LIBS 	= -I../instruction -I../parsing -I../utils
CFLAGS  = $(LIBS) -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -Ofast -c

.SUFFIXES: .c .o

.PHONY: all clean

all: libencode.a

libencode.a: encode.o encode_branch.o encode_data_proc.o encode_data_trans.o encode_mul.o
	ar rcs ../$@ $^

encode_data_proc.o: encode_data_proc.c
	$(CC) $(CFLAGS) encode_data_proc.c -o encode_data_proc.o

encode_mul.o: encode_mul.c
	$(CC) $(CFLAGS) encode_mul.c -o encode_mul.o

encode_data_trans.o: encode_data_trans.c
	$(CC) $(CFLAGS) encode_data_trans.c -o encode_data_trans.o

encode_branch.o: encode_branch.c
	$(CC) $(CFLAGS) encode_branch.c -o encode_branch.o

encode.o: encode.c
	$(CC) $(CFLAGS) encode.c -o encode.o

clean:
	rm -f $(wildcard *.o)
	rm -f libencode.a