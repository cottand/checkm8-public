CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -Ofast

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.c assembler.o writer.o symbol_table.o loader.o token.o token_stream.o encode.o instruction.o encode_data_proc.o encode_mul.o encode_data_trans.o encode_branch.o llist.o parser.o
	$(CC) $(CFLAGS) assemble.c assembler.o writer.o symbol_table.o loader.o token.o token_stream.o encode.o instruction.o encode_data_proc.o encode_mul.o encode_data_trans.o encode_branch.o llist.o parser.o -o assemble

emulate: emulate.c emulator.o loader.o pipeline.o decode.o execute.o instruction.o shift.o execute_data_proc.o execute_mul.o execute_data_trans.o execute_branch.o
	$(CC) $(CFLAGS) emulate.c emulator.o loader.o pipeline.o decode.o execute.o instruction.o shift.o execute_data_proc.o execute_mul.o execute_data_trans.o execute_branch.o -o emulate

ptest: ptest.c emulator.o loader.o pipeline.o decode.o execute.o instruction.o shift.o execute_data_proc.o execute_mul.o execute_data_trans.o execute_branch.o token.o token_stream.o encode.o encode_data_proc.o encode_mul.o encode_data_trans.o encode_branch.o llist.o parser.o symbol_table.o
	$(CC) $(CFLAGS) ptest.c emulator.o loader.o pipeline.o decode.o execute.o instruction.o shift.o execute_data_proc.o execute_mul.o execute_data_trans.o execute_branch.o token.o token_stream.o encode.o encode_data_proc.o encode_mul.o encode_data_trans.o encode_branch.o llist.o parser.o symbol_table.o -o ptest

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

execute_mul.o : execute_mul.c
	$(CC) $(CFLAGS) execute_mul.c -c -o execute_mul.o

execute_data_trans.o : execute_data_trans.c
	$(CC) $(CFLAGS) execute_data_trans.c -c -o execute_data_trans.o

execute_branch.o : execute_branch.c
	$(CC) $(CFLAGS) execute_branch.c -c -o execute_branch.o

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

encode_mul.o: encode_mul.c
	$(CC) $(CFLAGS) encode_mul.c -c -o encode_mul.o

encode_data_trans.o: encode_data_trans.c
	$(CC) $(CFLAGS) encode_data_trans.c -c -o encode_data_trans.o

encode_branch.o: encode_branch.c
	$(CC) $(CFLAGS) encode_branch.c -c -o encode_branch.o

token_stream.o: token_stream.c
	$(CC) $(CFLAGS) token_stream.c -c -o token_stream.o

token.o: token.c
	$(CC) $(CFLAGS) token.c -c -o token.o

llist.o: llist.c
	$(CC) $(CFLAGS) llist.c -c -o llist.o

parser.o: parser.c
	$(CC) $(CFLAGS) parser.c -c -o parser.o

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
