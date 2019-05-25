#include "decode.h"

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
  Data_Proc_Instr *ret =
    malloc(sizeof(Data_Proc_Instr));

}

Mul_Instr *decode_mul_instr(uint32_t instr)
{
  return malloc(sizeof(decode_mul_instr));
}

Data_Trans_Instr *decode_data_trans_instr(uint32_t instr)
{
}

Branch_Instr *decode_branch_instr(uint32_t instr)
{
  return malloc(decode_branch_instr);
}

instr_type *get_instr_type(uint32_t instr)
{
}
