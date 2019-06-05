#ifndef EXECUTE_H_
#define EXECUTE_H_

#include "emulator.h"
#include "instruction.h"

void exec_instr(Emulator *emulator, Instr *instr);

uint8_t is_cond_true(Emulator *emulator, uint8_t cond);
uint32_t compute_offset_from_reg(Emulator* emulator, uint16_t field, uint32_t *carry);

#endif /* EXECUTE_H_ */
