#include "pipeline.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "emulator.h"
#include "decode.h"
#include "execute.h"

void pipeline_init(Pipeline *pipeline, Emulator *emulator)
{
  pipeline->current_state = Empty;
  pipeline->emulator = emulator;
}

void cycle_first(Pipeline *pipeline)
{
  Emulator* em = pipeline->emulator;
  pipeline->fetching = fetch(em);
  inrc_PC(em);
}

void cycle_after_jump(Pipeline *pipeline)
{
  pipeline->decoded = decode_instr(pipeline->fetching);
  cycle_first(pipeline);
}

void cycle_normal(Pipeline *pipeline)
{
 pipeline->executing = pipeline->decoded;
 exec_instr(pipeline->emulator, &(pipeline->executing));
 cycle_after_jump(pipeline);
}


void cycle_p(Pipeline *pipeline)
{
  pipeline->fetching = fetch(pipeline->emulator);

  switch (pipeline->current_state)
  {
  case Empty:
    cycle_first(pipeline);
    break;
  case Half:
    cycle_after_jump(pipeline);
    break;
  case Full:
    cycle_normal(pipeline);
    break;
  default:
    /*Should never happen*/
    break;
  }


}
