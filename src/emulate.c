#include <stdlib.h>
#include "emulator.h"
#include "loader.h"

int main(int argc, char **argv)
{
  Emulator em;

  emulator_init(&em);
  load_binary(&em, argv[1]);

  print_mem(&em);
}
