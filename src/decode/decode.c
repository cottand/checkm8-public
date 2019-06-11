#include "decode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Instr decode_instr(uint32_t instr)
{
  Instr decoded;
  instr_init(&decoded);

  decoded.type = get_instr_type_from_uint(instr);

  switch (decoded.type)
  {
  case Data_Proc:
    decoded.data_proc_instr = decode_data_proc_instr(instr);
    break;
  case Mul:
    decoded.mul_instr = decode_mul_instr(instr);
    break;
  case Data_Trans:
    decoded.data_trans_instr = decode_data_trans_instr(instr);
    break;
  case Branch:
    decoded.branch_instr = decode_branch_instr(instr);
    break;
  case Halt:
    /* No decoding necessary */
    break;
  default:
    printf("Error: invalid instr type\n");
    break;
  }

  return decoded;
}

Data_Proc_Instr *decode_data_proc_instr(uint32_t instr)
{
  Data_Proc_Instr *decoded = malloc(sizeof(Data_Proc_Instr));

  decoded->cond = instr >> 28;
  decoded->i = instr >> 25;
  decoded->opcode = instr >> 21;
  decoded->s = instr >> 20;
  decoded->rn = instr >> 16;
  decoded->rd = instr >> 12;
  decoded->operand_2 = instr >> 0;

  return decoded;
}

Mul_Instr *decode_mul_instr(uint32_t instr)
{
  Mul_Instr *decoded = malloc(sizeof(Mul_Instr));

  decoded->cond = instr >> 28;
  decoded->a = instr >> 21;
  decoded->s = instr >> 20;
  decoded->rd = instr >> 16;
  decoded->rn = instr >> 12;
  decoded->rs = instr >> 8;
  decoded->rm = instr >> 0;

  return decoded;
}

Data_Trans_Instr *decode_data_trans_instr(uint32_t instr)
{
  Data_Trans_Instr *decoded = malloc(sizeof(Data_Trans_Instr));

  decoded->cond = instr >> 28;
  decoded->i = instr >> 25;
  decoded->p = instr >> 24;
  decoded->u = instr >> 23;
  decoded->l = instr >> 20;
  decoded->rn = instr >> 16;
  decoded->rd = instr >> 12;
  decoded->offset = instr >> 0;

  return decoded;
}

Branch_Instr *decode_branch_instr(uint32_t instr)
{
  Branch_Instr *decoded = malloc(sizeof(Branch_Instr));
  decoded->cond = instr >> 28;
  // Since offset 24 bits and instr, instr will be auto truncated as desired.
  decoded->offset = instr;

  return decoded;
}

instr_type get_instr_type_from_uint(uint32_t instr)
{
  // We found a halt instruction
  if (instr == 0x0)
  {
    return Halt;
  }

  uint32_t branch_mask = 0x0f000000;
  uint32_t branch_pattern = 0x0a000000;
  if (!((instr & branch_mask) ^ branch_pattern))
  {
    return Branch;
  }

  uint32_t data_trans_mask = 0x0c600000;
  uint32_t data_trans_pattern = 0x04000000;
  if (!((instr & data_trans_mask) ^ data_trans_pattern))
  {
    return Data_Trans;
  }

  uint32_t mult_mask = 0x0fc000f0;
  uint32_t mult_pattern = 0x00000090;
  if (!((instr & mult_mask) ^ mult_pattern))
  {
    return Mul;
  }

  uint32_t data_proc_mask = 0x0c000000;
  uint32_t data_proc_pattern = 0x00000000;
  if (!((instr & data_proc_mask) ^ data_proc_pattern))
  {
    return Data_Proc;
  }

  return -1;
}
