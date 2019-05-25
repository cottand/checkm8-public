#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdint.h>

typedef enum e_instr_type
{
  Data_Proc,
  Mul,
  Data_Trans,
  Branch
} instr_type;

typedef struct s_Decoded_Instr
{
  instr_type type;

  Data_Proc_Instr *data_proc_instr;
  Mul_Instr *mul_instr;
  Data_Trans_Instr *data_trans_instr;
  Branch_Instr *branch_instr;
} Decoded_Instr;

typedef struct s_Data_Proc_Instr
{
} Data_Proc_Instr;

typedef struct s_Mul_Instr
{
} Mul_Instr;

typedef struct s_Data_Trans_Instr
{
} Data_Trans_Instr;

typedef struct s_Branch_Instr
{
} Branch_Instr;

#endif /* INSTRUCTION_H_ */
