#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "emulator.h"
#include "loader.h"
#include "decode.h"
#include "execute.h"

int main(int argc, char **argv)
{
  Emulator em;
  emulator_init(&em);

  em.regs[0] = 0x00000001; // Rn
  em.regs[1] = 0x00000004; // Rs
  em.regs[2] = 0x00000004; // Rm

  uint32_t instr = 0xe0130192; // No acc
  //uint32_t instr = 0xe0330192; // acc

  Decoded_Instr decoded = decode_instr(instr);
  exec_instr(&em, &decoded);

  print_regs(&em);

  return EXIT_SUCCESS;
}
