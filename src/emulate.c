#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"
#include "loader.h"
#include "decode.h"

int main(int argc, char **argv)
{
  /*
  Emulator em;

  emulator_init(&em);
  load_binary(&em, argv[1]);

  print_mem(&em);
  */

  Mul_Instr *instr = malloc(sizeof(Mul_Instr));
  instr->cond = 0xf;

  printf("%x\n", instr->cond);

  return EXIT_SUCCESS;
}
