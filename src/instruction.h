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

typedef struct s_Data_Proc_Instr
{
  unsigned int cond : 4;
  unsigned int i : 1;
  unsigned int opcode : 4;
  unsigned int s : 1;
  unsigned int rn : 4;
  unsigned int rd : 4;
  unsigned int operand_2 : 12;

} Data_Proc_Instr;

typedef struct s_Mul_Instr
{
  unsigned int cond : 4;
  unsigned int a : 1;
  unsigned int s : 1;
  unsigned int rd : 4;
  unsigned int rn : 4;
  unsigned int rs : 4;
  unsigned int rm : 4;
} Mul_Instr;

typedef struct s_Data_Trans_Instr
{
  unsigned int cond : 4;
  unsigned int i : 1;
  unsigned int p : 1;
  unsigned int u : 1;
  unsigned int l : 1;
  unsigned int rn : 4;
  unsigned int rd : 4;
  unsigned int offset: 12;
} Data_Trans_Instr;

typedef struct s_Branch_Instr
{
  unsigned int cond : 4;
  unsigned int offset : 24;
} Branch_Instr;

typedef struct s_Decoded_Instr
{
  instr_type type;

  Data_Proc_Instr *data_proc_instr;
  Mul_Instr *mul_instr;
  Data_Trans_Instr *data_trans_instr;
  Branch_Instr *branch_instr;
} Decoded_Instr;

void decoded_instr_init(Decoded_Instr* instr);
void decoded_instr_free(Decoded_Instr* instr);

#endif /* INSTRUCTION_H_ */
