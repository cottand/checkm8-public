#include <stdlib.h>
#include "emulator.h"

#include <stdio.h>
#include <string.h>
#include "loader.h"
#include "pipeline.h"

int main(int argc, char **argv)
{
    Emulator em;
    emulator_init(&em);
    emulate(&em, argv[1]);
    print_regs(&em);
    print_mem(&em);
    return EXIT_SUCCESS;
}
