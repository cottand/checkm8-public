#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"
#include "execute.h"
#include "decode.h"

int main(int argc, char **argv)
{
  Emulator em;
  emulator_init(&em);
  emulate(&em, argv[1]);
  print_mem(&em);
  print_regs(&em);

  return EXIT_SUCCESS;
}
