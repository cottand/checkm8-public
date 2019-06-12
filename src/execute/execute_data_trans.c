#include "execute_data_trans.h"
#include "execute.h"
#include <stdio.h>

void exec_data_trans_instr(Emulator *emulator, Data_Trans_Instr *instr)
{
  if (!is_cond_true(emulator, instr->cond))
  {
    return;
  }

  uint32_t offset = 0;
  if (instr->i) /* Offset is shifted register */
  {
    uint32_t carry = 0;
    offset = compute_offset_from_reg(emulator, instr->offset, &carry);
  }
  else /* Offset is unsigned 12 bit immediate offset */
  {
    offset = instr->offset;
  }

  uint32_t base = emulator->regs[instr->rn];

  if (instr->p)
  {
    base = instr->u ? base + offset : base - offset;
  }


  uint32_t dst_src = emulator->regs[instr->rd];

  if (instr->l) /* Load from memory */
  {
    emulator->regs[instr->rd] = get_mem(emulator, base);
  }
  else /* Store to memory */
  {
    set_mem(emulator, base, dst_src);
  }

  /* Only support post indexing */
  if (!instr->p)
  {
    emulator->regs[instr->rn] = instr->u ? base + offset : base - offset;
  }

  /* Save the new values into regs */
  // emulator->regs[instr->rn] = base;
  // emulator->regs[instr->rd] = dst_src;
}
