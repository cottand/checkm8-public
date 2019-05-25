#include "decode.h"
#include <stdio.h>

Decoded_Instr decode_instr(uint32_t instr)
{
  Decoded_Instr decoded;
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
  return 0;
}

Mul_Instr *decode_mul_instr(uint32_t instr)
{
  return 0;
}

Data_Trans_Instr *decode_data_trans_instr(uint32_t instr)
{
  return 0;
}

Branch_Instr *decode_branch_instr(uint32_t instr)
{
  return 0;
}

instr_type get_instr_type(uint32_t instr)
{
  if (!(~instr & 0x0A000000))
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
