#include "execute_mul.h"
#include "execute.h"

void exec_mul_instr(Emulator *emulator, Mul_Instr *instr)
{
  if (!is_cond_true(emulator, instr->cond))
  {
    return;
  }

  uint32_t res = 0;
  uint32_t rn = emulator->regs[instr->rn];
  uint32_t rs = emulator->regs[instr->rs];
  uint32_t rm = emulator->regs[instr->rm];
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
