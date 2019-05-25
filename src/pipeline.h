#ifdef PIPELINE_H_
#define PIPELINE_H_

#include <stdint.h>
#include "emulator.h"
#include "decoded_instr.h"

enum p_state {
  Empty = 0;
  Half = 1;
  Full = 2;
}

typedef struct s_Pipeline
{
  uint16_t top;
  uint16_t med_coded;
  Decoded_Instr med_decoded;
  uint16_t bottom;
  enum p_state current_state;
} Pipeline;

void cycle(Pipeline *pipeline);


#endif
