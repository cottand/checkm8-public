CC       = gcc
CFLAGS   =  -Wall -g -std=c99 -Werror -pedantic -Ofast -lm
HEADERS  = -I/usr/local/include/opencv/ -I/usr/include/opencv2/
CLIBS	 = $(HEADERS) -L/usr/local/lib64 -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc

.SUFFIXES: .c .o

.PHONY: clean

all: vision

checkm8: main.c checkm8.o board.o cell.o board.o
	$(CC) $(CFLAGS) $^ -o $@

ptest: ptest.c snet.o
	$(CC) $(CFLAGS) $^ -o $@

checkm8.o : checkm8.c
	$(CC) $(CFLAGS) -c $^ -o $@

board.o: board.c
	$(CC) $(CFLAGS) -c $^ -o $@

cell.o: cell.c
	$(CC) $(CFLAGS)  -c $^ -o $@

vision: vision.c
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
