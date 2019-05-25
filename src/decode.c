#include "decode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ONES_SIZE1 1
#define ONES_SIZE4 0xf
#define ONES_SIZE12 0xfff
#define ONES_SIZE24 0xffffff


Decoded_Instr decode_instr(uint32_t instr)
{
  Decoded_Instr decoded;
  decoded_instr_init(&decoded);

  decoded.type = get_instr_type(instr);

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
  default:
    printf("Error: invalid instr type");
    break;
  }

  return decoded;
}

Data_Proc_Instr *decode_data_proc_instr(uint32_t instr)
{
  Data_Proc_Instr *decoded = malloc(sizeof(Data_Proc_Instr));

  /* TODO edit struct fields*/

  /* decoded->cond=.... */

  return decoded;
}

Mul_Instr *decode_mul_instr(uint32_t instr)
{
  Mul_Instr *decoded = malloc(sizeof(Mul_Instr));

  /* TODO edit struct fields*/

  /* decoded->cond=.... */

  return decoded;
}

Data_Trans_Instr *decode_data_trans_instr(uint32_t instr)
{
  Data_Trans_Instr *decoded = malloc(sizeof(Data_Trans_Instr));

  /* TODO edit struct fields*/

  /* decoded->cond=.... */

  return decoded;
}

Branch_Instr *decode_branch_instr(uint32_t instr)
{
  Branch_Instr *decoded = malloc(sizeof(Branch_Instr));

  /**
   * Bit into bitfield extraxtion example:
   * if we're looking for t bits at the nth position
   * from the right,
   * we shift right n and to a bitwise AND size t.
   * So if want the 4 bits at bit 28, we shift
   * right 28 and do AND 1111(binary) = 0xf
   * 
   * NOT TESTED :3
   */
  decoded->cond = (instr >> 28) & ONES_SIZE4;
  decoded->offset = (instr >> 0) & ONES_SIZE24;

  return decoded;
}

instr_type get_instr_type(uint32_t instr)
{
  if (!(~instr & 0x0a000000))
  {
    return Branch;
  }
  if (!(~instr & 0x04000000))
  {
    return Data_Trans;
  }
  if (!(~instr & 0x00000090))
  {
    return Mul;
  }
  if (!(instr & 0x0))
  {
    return Data_Proc;
  }

  return -1;
}
