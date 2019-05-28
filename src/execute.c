#include "execute.h"
#include <stdio.h>
#include "data_proc.h"

void exec_instr(Emulator *emulator, Decoded_Instr *instr)
{
  switch (instr->type)
  {
  case Data_Proc:
    exec_data_proc_instr(emulator, instr->data_proc_instr);
    break;
  case Mul:
    exec_mul_instr(emulator, instr->mul_instr);
    break;
  case Data_Trans:
    exec_data_trans_instr(emulator, instr->data_trans_instr);
    break;
  case Branch:
    exec_branch_instr(emulator, instr->branch_instr);
    break;
  }
}

// void exec_data_proc_instr(Emulator *emulator, Data_Proc_Instr *instr)
// {
  // if (!is_cond_true(emulator, instr->cond)) { return; }
// }

void exec_mul_instr(Emulator *emulator, Mul_Instr *instr)
{
  if (!is_cond_true(emulator, instr->cond)) { return; }

  uint32_t res = 0;
  uint32_t rn  = emulator->regs[instr->rn];
  uint32_t rs  = emulator->regs[instr->rs];
  uint32_t rm  = emulator->regs[instr->rm];
  if (instr->a)
  {
    res = rm * rs + rn;
  }
  else
  {
    res = rm * rs;
  }

  emulator->regs[instr->rd] = res;

  if (instr->s)
  {
    if (!res)
    {
      set_flag_Z(emulator);
    }
    else
    {
      clr_flag_Z(emulator);
    }

    if (get_reg_bit(emulator, instr->rd, 31))
    {
      set_flag_N(emulator);
    }
    else
    {
      clr_flag_N(emulator);
    }
  }
}

void exec_data_trans_instr(Emulator *emulator, Data_Trans_Instr *instr)
{
  if (!is_cond_true(emulator, instr->cond)) { return; }
}

void exec_branch_instr(Emulator *emulator, Branch_Instr *instr)
{
  if (!is_cond_true(emulator, instr->cond)) { return; }

  uint32_t offset = instr->offset << 2;
  uint32_t pc = get_PC(emulator);
  pc += offset;

  set_PC(emulator, pc);
}

uint8_t is_cond_true(Emulator *emulator, uint8_t cond)
{
  switch (cond)
  {
  case 0x0:
    return (get_flag_Z(emulator));
  case 0x1:
    return (!get_flag_Z(emulator));
  case 0xa:
    return (get_flag_V(emulator) == get_flag_N(emulator));
  case 0xb:
    return (get_flag_V(emulator) != get_flag_N(emulator));
  case 0xc:
    return (!get_flag_Z(emulator)
        && (get_flag_V(emulator) == get_flag_N(emulator)));
  case 0xd:
    return (get_flag_Z(emulator)
        || (get_flag_V(emulator) != get_flag_N(emulator)));
  case 0xe:
    return 1;
  default:
    printf("Error: invalid instruction condition code %x", cond);
    return 0;
  }
}
