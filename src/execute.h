#ifndef EXECUTE_H_
#define EXECUTE_H_

#include "emulator.h"
#include "instruction.h"

void exec_instr(Emulator *emulator, Decoded_Instr *instr);

// void exec_data_proc_instr(Emulator *emulator, Data_Proc_Instr *instr);
void exec_mul_instr(Emulator *emulator, Mul_Instr *instr);
void exec_data_trans_instr(Emulator *emulator, Data_Trans_Instr *instr);
void exec_branch_instr(Emulator *emulator, Branch_Instr *instr);

uint8_t is_cond_true(Emulator *emulator, uint8_t cond);

uint32_t compute_offset_from_reg(Emulator* emulator, uint16_t field);

#endif /* EXECUTE_H_ */
