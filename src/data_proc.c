#include "emulator.h"
#include "instruction.h"
#include "data_proc.h"
#include "execute.h"


static const struct BitBlock write_res_block = {.size = 2, .lsb_loc = 2};
static const struct BitBlock n_bit_block = {.size = 1, .lsb_loc = 31};
static const struct BitBlock Imm_block = {.size = 8, .lsb_loc = 0};
static const struct BitBlock Rm_block = {.size = 4, .lsb_loc = 0};
static const struct BitBlock use_reg_block = {.size = 1, .lsb_loc = 0};
static const struct BitBlock shift_type_block = {.size = 2, .lsb_loc = 1};
static const struct BitBlock integer_block = {.size = 5, .lsb_loc = 3};
static const struct BitBlock Rs_block = {.size = 4, .lsb_loc = 4};
static const struct BitBlock Rs_bottom_byte_block = {.size = 8, .lsb_loc = 0};

void exec_data_proc_instr(Emulator *emulator, Data_Proc_Instr *instr)
{
  if (!is_cond_true(emulator, instr->cond))
  {
    return;
  }

  int c_flag = 0;

  int32_t op1 = emulator->regs[instr->rn];  
  int32_t op2 = get_operand2(emulator, instr->operand_2, instr->i, &c_flag);
  int32_t res = 0;

  enum e_operation operator= decode_opcode(instr->opcode);

  if (operator== and || operator== eor || operator== orr || operator== teq ||
      operator== tst ||
      operator== mov)
  {
    if (c_flag)
    {
      set_flag_C(emulator);
    }
    else
    {
      clr_flag_C(emulator);
    }
  }

  clr_flag_C(emulator);
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
    res = op1 ^ op2;
    break;
  case mov:
    res = op2;
    break;
  case add:
    res_64 = op1 + op2;
    if (res_64 > INT32_MAX)
    {
      set_flag_C(emulator);
    }
    res = (uint32_t)res_64;
    break;
  case sub:
    res_64 = op1 - op2;
    if (res_64 < INT32_MIN)
    {
      set_flag_C(emulator);
    }
    res = (uint32_t)res_64;
    break;
  case rsb:
    res_64 = op2 - op1;
    if (res_64 < INT32_MIN)
    {
      set_flag_C(emulator);
    }
    res = (uint32_t)res_64;
    break;
  case cmp:
    res_64 = op1 - op2;
    if (res_64 < INT32_MIN)
    {
      set_flag_C(emulator);
    }
    res = (uint32_t)res_64;
    break;

  default:
    break;
  }

  //Set CPSR
  if (!instr->s)
  {
    if (res == 0)
    {
      set_flag_Z(emulator);
    }

    if (get_bit_block(res, n_bit_block, 0))
    {
      set_flag_N(emulator);
    }
    else
    {
      clr_flag_N(emulator);
    }
  }

  int write_result = get_bit_block(instr->opcode, write_res_block, 0) != 2;
  if (write_result)
  {
    emulator->regs[instr->rd] = res;
  }
}

int32_t get_operand2(Emulator *emulator, unsigned int op2, unsigned int I_flag,
                      int *carry)
{
  if (I_flag)
  {
    uint32_t imm = get_bit_block(op2, Imm_block, 0);
    uint8_t rotate = get_bit_block(op2, Imm_block, 1);
    return rotate_right(imm, rotate * 2, carry);
  }

  unsigned int Rm = get_bit_block(op2, Rm_block, 0);
  unsigned int Shift = get_bit_block(op2, Rm_block, 1);
  uint32_t Rm_value = emulator->regs[Rm];

  uint8_t use_reg = get_bit_block(Shift, use_reg_block, 0);

  unsigned int shift_shift_amount = 0;

  //Shift by a constant shift_amount
  if (!use_reg)
  {
    unsigned int integer = get_bit_block(Shift, integer_block, 1);
    shift_shift_amount = integer;
  }
  //Shift specified by a register
  else
  {
    unsigned int Rs = get_bit_block(Shift, Rs_block, 1);
    shift_shift_amount = get_bit_block(emulator->regs[Rs], Rs_bottom_byte_block, 0);
  }

  unsigned int shift_code = get_bit_block(Shift, shift_type_block, 0);
  int32_t op2_val = 0;

  switch (decode_shift_type(shift_code))
  {
  case lsl:
    op2_val = shift_logical_left(Rm_value, shift_shift_amount, carry);
    break;

  case lsr:
    op2_val = shift_logical_right(Rm_value, shift_shift_amount, carry);
    break;

  case asr:
    op2_val = shift_arithmetic_right(Rm_value, shift_shift_amount, carry);
    break;

  case ror:
    op2_val = rotate_right(Rm_value, shift_shift_amount, carry);
    break;

  default:
    break;
  }

  return op2_val;
}


unsigned int make_block_mask(struct BitBlock block)
{
  unsigned int mask = (1 << block.size) - 1;
  return mask << block.lsb_loc;
}

unsigned int get_bit_block(unsigned int original, struct BitBlock block,
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

uint32_t rotate_right(uint32_t original, int shift_amount, int *carry)
{
  *carry = (original >> shift_amount) & 1;
  uint32_t shifted = original >> shift_amount;
  uint32_t rotated = original << (32 - shift_amount);

  return shifted | rotated;
}

uint32_t shift_logical_left(uint32_t original, unsigned int shift_shift_amount,
                            int *carry)
{
  *carry = (original >> shift_shift_amount) & 1;
  return original << shift_shift_amount;
}

uint32_t shift_logical_right(uint32_t original, unsigned int shift_shift_amount,
                             int *carry)
{
  *carry = (original >> (32 - shift_shift_amount)) & 1;
  return original << shift_shift_amount;
}

uint32_t shift_arithmetic_right(int original, unsigned int shift_shift_amount,
                                int *carry)
{
  *carry = ((unsigned)original >> (32 - shift_shift_amount)) & 1;
  return original >> shift_shift_amount;
}

enum e_shift_type decode_shift_type(unsigned int code)
{
  return (enum e_shift_type)code;
}

enum e_operation decode_opcode(unsigned int code)
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
