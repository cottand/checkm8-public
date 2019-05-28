#ifndef DATA_PROC_H_
#define DATA_PROC_H_

#include "emulator.h"
#include "instruction.h"

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
} operation;

typedef struct s_Bit_Block
{
  uint32_t size;
  uint32_t lsb_loc;
} Bit_Block;


void exec_data_proc_instr(Emulator *emulator, Data_Proc_Instr *instr);

unsigned int make_block_mask(struct s_Bit_Block block);
uint32_t get_operand2(Emulator *emulator, unsigned int op2, unsigned int I_flag,
                      int *carry);
unsigned int get_bit_block(unsigned int original, struct s_Bit_Block block,
                           int invert);
uint32_t rotate_right(uint32_t original, int shift_amount, int *carry);
uint32_t shift_logical_left(uint32_t original, unsigned int shift_shift_amount,
                            int *carry);
uint32_t shift_logical_right(uint32_t original, unsigned int shift_shift_amount,
                             int *carry);
uint32_t shift_arithmetic_right(int original, unsigned int shift_shift_amount,
                                int *carry);
operation decode_opcode(unsigned int code);

#endif /* DATA_PROC_H_ */
