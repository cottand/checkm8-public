#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "pipeline.h"
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
  Emulator *em = pipeline->emulator;
  pipeline->fetching = fetch(em);
  incr_PC(em);
  pipeline->current_state = Half;
  pipeline->addresses_bottom_to_top[0] = pipeline->fetching;
}

void cycle_after_jump(Pipeline *pipeline)
{
  pipeline->decoded = decode_instr(pipeline->fetching);
  cycle_first(pipeline);
  pipeline->current_state = Full;
  pipeline->addresses_bottom_to_top[1] = pipeline->to_decode;
}

void cycle_normal(Pipeline *pipeline)
{
  pipeline->executing = pipeline->decoded;
  exec_instr(pipeline->emulator, &(pipeline->executing));
  cycle_after_jump(pipeline);
  if (pipeline->executing.type == Branch && is_cond_true(pipeline->emulator, pipeline->executing.branch_instr->cond))
  {
    pipeline->current_state = Half;
  }
  pipeline->addresses_bottom_to_top[2] = pipeline->addresses_bottom_to_top[1];
}

void cycle_p(Pipeline *pipeline)
{
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
    instr_free(&(pipeline->executing));
    break;
  default:
    /*Should never happen*/
    break;
  }
}
