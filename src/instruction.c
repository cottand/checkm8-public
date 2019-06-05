#include "instruction.h"
#include "emulator.h"

#include <stdlib.h>
#include <stdint.h>

void instr_init(Instr* instr)
{
  instr->data_proc_instr  = 0;
  instr->mul_instr        = 0;
  instr->data_trans_instr = 0;
  instr->branch_instr     = 0;
}

void instr_free(Instr* instr)
{
  if (instr->data_proc_instr)
  {
    free(instr->data_proc_instr);
  }
  if (instr->mul_instr)
  {
    free(instr->mul_instr);
  }
  if (instr->data_trans_instr)
  {
    free(instr->data_trans_instr);
  }
  if (instr->branch_instr)
  {
    free(instr->branch_instr);
  }
}

uint32_t instr_to_uint32(Instr *instr)
{
  switch (instr->type)
  {
  case Data_Proc:
    return data_proc_instr_to_uint32(instr->data_proc_instr);
    break;
  case Mul:
    return mul_instr_to_uint32(instr->mul_instr);
    break;
  case Data_Trans:
    return data_trans_instr_to_uint32(instr->data_trans_instr);
    break;
  case Branch:
    return branch_instr_to_uint32(instr->branch_instr);
    break;
  case Halt:
    return halt_instr_to_uint32();
    break;
  }

  return 0x0;
}

uint32_t data_proc_instr_to_uint32(Data_Proc_Instr *instr)
{
  uint32_t bin = instr->cond << 28;
  bin |= instr->i << 25;
  bin |= instr->opcode << 21;
  bin |= instr->s << 20;
  bin |= instr->rn << 16;
  bin |= instr->rd << 12;
  bin |= instr->operand_2;

  return bin;
}

uint32_t mul_instr_to_uint32(Mul_Instr *instr)
{
  uint32_t bin = instr->cond << 28;
  bin |= instr->a << 21;
  bin |= instr->s << 20;
  bin |= instr->rd << 16;
  bin |= instr->rn << 12;
  bin |= instr->rs << 8;
  bin |= 0x9 << 4;
  bin |= instr->rm;

  return bin;
}

uint32_t data_trans_instr_to_uint32(Data_Trans_Instr *instr)
{
  uint32_t bin = instr->cond << 28;
  bin |= 0x1 << 26;
  bin |= instr->i << 25;
  bin |= instr->p << 24;
  bin |= instr->u << 23;
  bin |= instr->l << 20;
  bin |= instr->rn << 16;
  bin |= instr->rd << 12;
  bin |= instr->offset;

  return bin;
}

uint32_t branch_instr_to_uint32(Branch_Instr *instr)
{
  uint32_t bin = instr->cond << 28;
  bin |= 0xa << 24;
  bin |= ((uint32_t) instr->offset) & 0x00ffffff;

  return bin;
}

uint32_t halt_instr_to_uint32()
{
  return 0x0;
}
