#ifndef DECODE_H_
#define DECODE_H_

#include "instruction.h"

Decoded_Instr decode_instr(uint32_t instr);

Data_Proc_Instr decode_data_proc_instr(uint32_t instr);
Mul_Instr decode_mul_instr(uint32_t instr);
Data_Trans_Instr decode_data_trans_instr(uint32_t instr);
Branch_Instr decode_branch_instr(uint32_t instr);

instr_type get_instr_type(uint32_t instr);

#endif /* DECODE_H_ */
