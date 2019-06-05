CC      = gcc
LIBS	= -I.. -I../instruction -I../utils
CFLAGS  = $(LIBS) -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -Ofast -c

.SUFFIXES: .c .o

.PHONY: all clean

all: execute.a

execute.a: execute.o execute_branch.o execute_data_proc.o execute_data_trans.o execute_mul.o
	ar rcs ../$@ $^

execute.o: execute.c
	$(CC) $(CFLAGS) execute.c -c -o execute.o

execute_data_proc.o: execute_data_proc.c
	$(CC) $(CFLAGS) execute_data_proc.c -c -o execute_data_proc.o

execute_mul.o : execute_mul.c
	$(CC) $(CFLAGS) execute_mul.c -c -o execute_mul.o

execute_data_trans.o : execute_data_trans.c
	$(CC) $(CFLAGS) execute_data_trans.c -c -o execute_data_trans.o

execute_branch.o : execute_branch.c
	$(CC) $(CFLAGS) execute_branch.c -c -o execute_branch.o


clean:
	rm -f $(wildcard *.o)
	rm -f execute.a