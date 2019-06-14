CC       = gcc
CFLAGS   = $(CLIBS) -Wall -g -Werror -std=c99 -pedantic -Ofast 
HEADERS  = -I/usr/local/include/opencv/ -I/usr/include/opencv2/
CLIBS	 = $(HEADERS) 
VFLAGS =  -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -L/usr/local/lib64

.SUFFIXES: .c .o

.PHONY: clean

all: checkm8

checkm8: main.c checkm8.o board.o cell.o board.o vision.o snet.o
	$(CC) $(CFLAGS) $(VFLAGS) $^ -o $@

ptest: ptest.c snet.o
	$(CC) $(CFLAGS) $^ -o $@

checkm8.o : checkm8.c
	$(CC) $(CFLAGS) -c $^ -o $@

board.o: board.c
	$(CC) $(CFLAGS) -c $^ -o $@

cell.o: cell.c
	$(CC) $(CFLAGS)  -c $^ -o $@

vision.o: vision.c
	$(CC) $(CFLAGS) -c $^ -o $@

snet.o: singnet.c
	$(CC) $(CFLAGS)  -c $^ -o $@

clean:
	rm -f $(wildcard *.o)
	rm -f $(wildcard *.a)
	rm -f checkm8
	rm -f ptest
	rm -f vision
	rm -f snet
