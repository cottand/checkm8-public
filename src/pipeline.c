#include "pipeline.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "emulator.h"

Pipeline pipeline_init(Emulator *emulator, Pipeline *pipeline)
{
  pipeline->current_state = Empty;
  pipeline->emulator = emulator;
}

void cycle(Pipeline *pipeline){
}