#ifndef ENCODE_DATA_PROC_H_
#define ENCODE_DATA_PROC_H_

#include "instruction.h"
#include "token_stream.h"

Data_Proc_Instr *encode_data_proc_instr(Token_Stream *instr);

/* For opcodes: and eor sub rsb add orr */
void encode_compute(Data_Proc_Instr *instr, Token_Stream *tokens);

/* For opcode: mov */
void encode_mov(Data_Proc_Instr *instr, Token_Stream *tokens);

/* For opcodes: tst teq cmp */
void encode_test(Data_Proc_Instr *instr, Token_Stream *tokens);

void encode_operand2(Data_Proc_Instr *instr, Token_Stream *tokens);
void encode_operand2_immediate(Data_Proc_Instr *instr, Token_Stream *tokens);
void encode_operand2_register(Data_Proc_Instr *instr, Token_Stream *tokens);

uint8_t encode_operand2_shift(char *shift);
uint32_t encode_immediate(char *immediate);

#endif /* ENCODE_DATA_PROC_H_ */
