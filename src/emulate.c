#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "emulator.h" it's in pipeline
#include "loader.h"
//#include "decode.h"  in pipeline
//#include "execute.h"   in pipeline
#include "pipeline.h"

int main(int argc, char **argv)
{
  Emulator em;
  emulator_init(&em);
  load_binary(&em, argv[1]);
/*
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
*/
/* Fetch testing rubbish

  print_regs(&em);

  print_mem(&em);
  uint16_t pc_addr = (uint16_t) get_PC(&em);
  printf("\nPC contains %x atm \n", pc_addr);
  uint8_t i_bits[4];
  for(int i=0; i<4; i++){
    i_bits[i] = get_mem_byte(&em, pc_addr+(3-i));
    printf("\n Current instrbit n. %d at %x is %x\n", i, (pc_addr+i), i_bits[i]);
  }
  uint32_t* instr = (uint32_t*) &i_bits[0];
  printf("Instruction therefore is: %x \n", *instr);

*/
  Pipeline pip;
  pipeline_init(&pip, &em);
  print_regs(&em);
  for(int i = 0; i<3; i++)
  {
  cycle_p(&pip);

  }
  print_regs(&em);
  print_mem(&em);
  return EXIT_SUCCESS;
}
