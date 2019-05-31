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
