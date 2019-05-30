#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <stdint.h>
#include "emulator.h"
#include "instruction.h"

typedef enum e_p_state
{
  Empty = 0,
  Half = 1,
  Full = 2,
} p_state;

typedef struct s_Pipeline
{
  Instr executing;
  Instr decoded;
  uint32_t to_decode;
  uint32_t fetching;
  p_state current_state;
  Emulator *emulator;
  uint16_t addresses_bottom_to_top[3];
} Pipeline;


void pipeline_init(Pipeline *pipeline, Emulator *emulator);

void cycle_p(Pipeline *pipeline);

#endif /* PIPELINE_H_ */
