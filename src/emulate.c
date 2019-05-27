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
  load_binary(&em, argv[1]);

  em.regs[0] = 0x00000001; // Rn
  em.regs[1] = 0x00000004; // Rs
  em.regs[2] = 0x00000004; // Rm

  // Mul instructions
  //uint32_t instr = 0xe0130192; // No acc
  //uint32_t instr = 0xe0330192; // acc

  // Branch instructions
  //uint32_t instr = 0xea000001;

  // Data trans instructions
  //uint32_t instr = 0xe5140000; // Load mem[regs[0]] into regs[4] without offset
  //uint32_t instr = 0xe5010000; // Store regs[1] into mem[regs[0]] without offset

  Decoded_Instr decoded = decode_instr(instr);
  exec_instr(&em, &decoded);

  print_regs(&em);
  print_mem(&em);

  return EXIT_SUCCESS;
}
