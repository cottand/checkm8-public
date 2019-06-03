#include "execute_branch.h"
#include "execute.h"

void exec_branch_instr(Emulator *emulator, Branch_Instr *instr)
{
  if (!is_cond_true(emulator, instr->cond))
  {
    return;
  }

  uint32_t offset = (uint32_t)(instr->offset << 2);
  uint32_t pc = get_PC(emulator);
  pc += offset;

  set_PC(emulator, pc);
}
