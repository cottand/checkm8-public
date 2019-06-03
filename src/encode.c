#include "encode.h"
#include "encode_data_proc.h"
#include "encode_mul.h"
#include "encode_data_trans.h"
#include "encode_branch.h"

#include <stdio.h>
#include <string.h>

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
  case Halt:
    break;
  default:
    printf("Invalid instr type");
    break;
  }

  return encoded;
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

  if (!strcmp(v, "andeq"))
  {
    return Halt;
  }

  return -1;
}
