#include "encode_mul.h"
#include <stdlib.h>
#include <string.h>

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
