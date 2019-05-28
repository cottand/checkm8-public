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
  Pipeline pip;
  pipeline_init(&pip, &em);
  print_regs(&em);
  print_mem(&em);
  return EXIT_SUCCESS;
}
