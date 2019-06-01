#include "encode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encode_data_proc.h"

Instr encode_instr(Token_Stream *instr)
{
  Instr encoded;
  instr_init(&encoded);

  encoded.type = get_instr_type_from_tok(token_stream_peak(instr));

  switch (encoded.type)
  {
  case Data_Proc:
    encoded.data_proc_instr = encode_data_proc_instr(instr);
    break;
  case Mul:
    encoded.mul_instr = encode_mul_instr(instr);
    break;
  case Data_Trans:
    encoded.data_trans_instr = encode_data_trans_instr(instr);
    break;
  case Branch:
    encoded.branch_instr = encode_branch_instr(instr);
    break;
  default:
    printf("Invalid instr type");
    break;
  }

  return encoded;
}

Mul_Instr *encode_mul_instr(Token_Stream *instr)
{
  Mul_Instr *encoded = malloc(sizeof(Mul_Instr));

  Token *opcode = token_stream_expect(instr, Opcode, "Expecting opcode for mul type instr (mul, mla)");

  Token *rd = token_stream_expect(instr, Register, "Expecting Rd register for mul instr");
  Token *rm = token_stream_expect(instr, Register, "Expecting Rm register for mul instr");
  Token *rs = token_stream_expect(instr, Register, "Expecting Rs register for mul instr");

  encoded->cond = 0xe;
  encoded->a  = 0x0;
  encoded->s  = 0x0;
  encoded->rd = strtoul(rd->value, 0, 10);
  encoded->rm = strtoul(rm->value, 0, 10);
  encoded->rs = strtoul(rs->value, 0, 10);
  encoded->rn = 0x0;

  if (!strcmp(opcode->value, "mla"))
  {
    Token *rn = token_stream_expect(instr, Register, "Expecting Rn register for mla instr");
    encoded->rn = strtoul(rn->value, 0, 10);

    encoded->a = 0x1;
  }

  return encoded;
}

Data_Trans_Instr *encode_data_trans_instr(Token_Stream *instr)
{
  return 0;
}

Branch_Instr *encode_branch_instr(Token_Stream *instr)
{
  return 0;
}

instr_type get_instr_type_from_tok(Token *tok)
{
  char* v = tok->value;

  if (!strcmp(v, "add") || !strcmp(v, "sub") ||
      !strcmp(v, "rsb") || !strcmp(v, "and") ||
      !strcmp(v, "eor") || !strcmp(v, "orr") ||
      !strcmp(v, "mov") || !strcmp(v, "tst") ||
      !strcmp(v, "teq") || !strcmp(v, "cmp"))
  {
    return Data_Proc;
  }

  if (!strcmp(v, "mul") || !strcmp(v, "mla"))
  {
    return Mul;
  }

  if (!strcmp(v, "ldr") || !strcmp(v, "str"))
  {
    return Data_Trans;
  }


  if (!strcmp(v, "beq") || !strcmp(v, "bne") ||
      !strcmp(v, "bge") || !strcmp(v, "blt") ||
      !strcmp(v, "bgt") || !strcmp(v, "b"))
  {
    return Branch;
  }

  return -1;
}
