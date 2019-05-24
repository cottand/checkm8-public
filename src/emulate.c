#include <stdlib.h>
#include "Emulator.h"
#include "Loader.h"

int main(int argc, char **argv)
{
  char *path = argv[1];
  Emulator em;
  
  print_mem(&em);

  emulator_init(&em);

  print_mem(&em);
}
