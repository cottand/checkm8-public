#include <stdlib.h>
#include "Emulator.h"
<<<<<<< HEAD
#include "Loader.h"
=======
>>>>>>> 9d34dc84bb15077cf8c3e5d92a424ce8692b7f8d

int main(int argc, char **argv)
{
  char *path = argv[1];
  Emulator em;
  
  print_mem(&em);

  emulator_init(&em);

  print_mem(&em);
}
