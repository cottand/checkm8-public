#include "checkm8.h"
#include <stdio.h>

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("Error: expecting cli argument for camera IP address\n");
  }

  checkm8_run(argv[1]);

  return 0;
}
