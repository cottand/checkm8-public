#ifndef DECODED_INSTR_H_
#define DECODED_INSTR_H_

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

#endif /* DECODED_INSTR_H_ */
