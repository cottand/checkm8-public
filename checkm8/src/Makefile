CC       = gcc
CFLAGS   = $(CLIBS) -Wall -g -std=c99 -pedantic -Ofast
HEADERS  = -I/usr/local/include/opencv/ -I/usr/include/opencv2/
CLIBS	 = $(HEADERS) -L/usr/local/lib64 -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -Wl,-rpath=/usr/local/lib64/

.SUFFIXES: .c .o

.PHONY: clean

all: vision

checkm8: main.c checkm8.o board.o cell.o
	$(CC) $(CFLAGS) main.c checkm8.o cell.o -o checkm8

ptest: ptest.c checkm8.o board.o cell.o
	$(CC) $(CFLAGS) ptest.c checkm8.o board.o cell.o -o ptest

checkm8.o : checkm8.c
	$(CC) $(CFLAGS) checkm8.c -c -o checkm8.o

board.o: board.c
	$(CC) $(CFLAGS) board.c -c -o board.o

cell.o: cell.c
	$(CC) $(CFLAGS) cell.c -c -o cell.o

vision: vision.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(wildcard *.o)
	rm -f $(wildcard *.a)
	rm -f checkm8
	rm -f ptest
	rm -f vision
