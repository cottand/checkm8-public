#ifndef DATA_PROC_H_
#define DATA_PROC_H_

#include "emulator.h"
#include "instruction.h"


typedef enum e_shift_type
{
  lsl,
  lsr,
  asr,
  ror
}temp1;


typedef enum e_operation
{
  and,
  eor,
  sub,
  rsb,
  add,
  tst,
  teq,
  cmp,
  orr,
  mov
}temp2;

typedef struct BitBlock {
   unsigned int size;
   unsigned int lsb_loc;
}temp3;


void exec_data_proc_instr(Emulator *emulator, Data_Proc_Instr *instr);
unsigned int make_block_mask(struct BitBlock block);
int32_t get_operand2(Emulator *emulator, unsigned int op2, unsigned int I_flag,
                      int *carry);
unsigned int get_bit_block(unsigned int original, struct BitBlock block,
                           int invert);
uint32_t rotate_right(uint32_t original, int shift_amount, int *carry);
uint32_t shift_logical_left(uint32_t original, unsigned int shift_shift_amount,
                            int *carry);
uint32_t shift_logical_right(uint32_t original, unsigned int shift_shift_amount,
                             int *carry);
uint32_t shift_arithmetic_right(int original, unsigned int shift_shift_amount,
                                int *carry);
enum e_shift_type decode_shift_type(unsigned int code);
enum e_operation decode_opcode(unsigned int code);

#endif /* DATA_PROC_H_ */