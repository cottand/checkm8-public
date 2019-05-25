#include "pipeline.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "emulator.h"

void pipeline_init(Pipeline *pipeline, Emulator *emulator)
{
  pipeline->current_state = Empty;
  pipeline->emulator = emulator;
}

void cycle_first(Pipeline *pipeline)
{
  /* TODO */
}

void cycle_after_jump(Pipeline *Pipeline)
{
  /* TODO */
}

void cycle_normal(Pipeline *Pipeline)
{
 /* TODO */
}


void cycle(Pipeline *pipeline)
{
  /* The fetch step is common to all cycles.
    I am not sure whether incrementing the PC is too??
    If so, is it after or before the pipeline cycle?
   */
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
