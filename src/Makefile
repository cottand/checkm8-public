CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.c assembler.o writer.o symbol_table.o loader.o token.o token_stream.o encode.o instruction.o encode_data_proc.o
	$(CC) $(CFLAGS) assemble.c assembler.o writer.o symbol_table.o loader.o token.o token_stream.o encode.o instruction.o encode_data_proc.o -o assemble

emulate: emulate.c emulator.o loader.o pipeline.o decode.o execute.o instruction.o shift.o execute_data_proc.o
	$(CC) $(CFLAGS) emulate.c emulator.o loader.o pipeline.o decode.o execute.o instruction.o shift.o execute_data_proc.o -o emulate

ptest: ptest.c emulator.o loader.o pipeline.o decode.o execute.o instruction.o shift.o execute_data_proc.o token.o token_stream.o encode.o encode_data_proc.o
	$(CC) $(CFLAGS) ptest.c emulator.o loader.o pipeline.o decode.o execute.o instruction.o shift.o execute_data_proc.o token.o token_stream.o encode.o encode_data_proc.o -o ptest

emulator.o: emulator.c
	$(CC) $(CFLAGS) emulator.c -c -o emulator.o

loader.o: loader.c
	$(CC) $(CFLAGS) loader.c -c -o loader.o

pipeline.o: pipeline.c
	$(CC) $(CFLAGS) pipeline.c -c -o pipeline.o

decode.o: decode.c
	$(CC) $(CFLAGS) decode.c -c -o decode.o

execute.o: execute.c
	$(CC) $(CFLAGS) execute.c -c -o execute.o

execute_data_proc.o: execute_data_proc.c
	$(CC) $(CFLAGS) execute_data_proc.c -c -o execute_data_proc.o

instruction.o: instruction.c
	$(CC) $(CFLAGS) instruction.c -c -o instruction.o

shift.o: shift.c
	$(CC) $(CFLAGS) shift.c -c -o shift.o

assembler.o: assembler.c
	$(CC) $(CFLAGS) assembler.c -c -o assembler.o

writer.o: writer.c
	$(CC) $(CFLAGS) writer.c -c -o writer.o

symbol_table.o: symbol_table.c
	$(CC) $(CFLAGS) symbol_table.c -c -o symbol_table.o

encode.o: encode.c
	$(CC) $(CFLAGS) encode.c -c -o encode.o

encode_data_proc.o: encode_data_proc.c
	$(CC) $(CFLAGS) encode_data_proc.c -c -o encode_data_proc.o

token_stream.o: token_stream.c
	$(CC) $(CFLAGS) token_stream.c -c -o token_stream.o

token.o: token.c
	$(CC) $(CFLAGS) token.c -c -o token.o

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
