#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <stdint.h>
#include "emulator.h"
#include "decoded_instr.h"

enum p_state {
  Empty = 0,
  Half = 1,
  Full = 2,
};

typedef struct s_Pipeline
{
  Decoded_Instr executing;
  Decoded_Instr decoded;
  uint16_t to_decode;
  uint16_t fetching;
  enum p_state current_state;
  Emulator *emulator;
} Pipeline;


void pipeline_init(Pipeline *pipeline, Emulator *emulator);

void cycle_p(Pipeline *pipeline);

#endif
