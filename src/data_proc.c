#include "emulator.h"
#include "instruction.h"
#include "data_proc.h"
#include "execute.h"
#include "shift.h"

#include <stdio.h>

void exec_data_proc_instr(Emulator *emulator, Data_Proc_Instr *instr)
{
  if (!is_cond_true(emulator, instr->cond))
  {
    return;
  }
  uint32_t c_flag = 1;
  uint32_t c_shift = 0;

  int32_t op1 = emulator->regs[instr->rn];
  int32_t op2 = get_operand2(emulator, instr->operand_2, instr->i, &c_shift);
  int32_t res = 0;
  operation operator= decode_opcode(instr->opcode);

  if (operator== and || operator== eor || operator== teq || operator== tst ||
      operator== mov)
  {
    c_flag = c_shift;
  }

  int64_t res_64 = 0;

  switch (operator)
  {
  case and:
    res = op1 & op2;
    break;
  case eor:
    res = op1 ^ op2;
    break;
  case orr:
    res = op1 | op2;
    break;
  case teq:
    res = op1 ^ op2;
    break;
  case tst:
    res = op1 & op2;
    break;
  case mov:
    res = op2;
    break;

  case add:
    res_64 = (uint64_t)op1 + (uint64_t)op2;
    if (res_64 > INT32_MAX)
    {
      set_flag_C(emulator);
    }
    res = (uint32_t)res_64;
    break;
  case cmp:
  case sub:

    printf("op1: %x  op2: %x \n", op1, op2);
    res_64 = op1 - op2;
    c_flag = op1 >= op2;
    res = (uint32_t)res_64;
    printf("c_flag: %x\n", c_flag);
    printf("res: %x\n", res);
    break;
  case rsb:
    res_64 = op2 - op1;
    c_flag = op2 >= op1;
    res = (uint32_t)res_64;
    break;

  default:
    break;
  }

  //Set CPSR
  if (instr->s)
  {
    if (res == 0)
    {
      set_flag_Z(emulator);
    }
    else
    {
      clr_flag_Z(emulator);
    }

    if (res >> 31)
    {
      set_flag_N(emulator);
    }
    else
    {
      clr_flag_N(emulator);
    }

    if (c_flag)
    {
      set_flag_C(emulator);
    }
    else
    {
      clr_flag_C(emulator);
    }
  }
  uint8_t write_result = (instr->opcode >> 2) != 2;

  if (write_result)
  {
    emulator->regs[instr->rd] = res;
  }
}

uint32_t get_operand2(Emulator *emulator, uint16_t op2, uint32_t I_flag,
                      uint32_t *carry)
{
  if (I_flag)
  {
    uint32_t immediate = op2 & 0xff;
    uint32_t rotate = op2 >> 8;
    rotate *= 2;

    return ror(immediate, rotate, carry);
  }

  return compute_offset_from_reg(emulator, op2, carry);
}

unsigned int make_block_mask(struct s_Bit_Block block)
{
  unsigned int mask = (1 << block.size) - 1;
  return mask << block.lsb_loc;
}

unsigned int get_bit_block(unsigned int original, struct s_Bit_Block block,
                           int invert)
{
  unsigned int mask = make_block_mask(block);

  if (invert)
  {
    mask = ~mask;
  }

  unsigned int bit_block = (original >> block.lsb_loc) & mask;
  return bit_block;
}

operation decode_opcode(uint8_t code)
{
  switch (code)
  {
  case 0:
    return and;
  case 1:
    return eor;
  case 2:
    return sub;
  case 3:
    return rsb;
  case 4:
    return add;
  case 8:
    return tst;
  case 9:
    return teq;
  case 10:
    return cmp;
  case 12:
    return orr;
  case 13:
    return mov;

  default:
    return -1;
  }
}
