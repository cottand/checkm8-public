#include "encode_branch.h"
#include <stdlib.h>
#include <string.h>

Branch_Instr *encode_branch_instr(Token_Stream *instr)
{
  Branch_Instr *encoded = malloc(sizeof(Branch_Instr));

  Token *opcode = token_stream_expect(instr, Opcode, "Expecting opcode for branch type instr");
  Token *offset = token_stream_expect(instr, Address, "Expecting an address for branch type instr");

  encoded->offset = strtoul(offset->value, 0, 16);

  if (!strcmp(opcode->value, "beq"))
  {
    encoded->cond = 0x0;
  }
  if (!strcmp(opcode->value, "bne"))
  {
    encoded->cond = 0x1;
  }
  if (!strcmp(opcode->value, "bge"))
  {
    encoded->cond = 0xa;
  }
  if (!strcmp(opcode->value, "blt"))
  {
    encoded->cond = 0xb;
  }
  if (!strcmp(opcode->value, "bgt"))
  {
    encoded->cond = 0xc;
  }
  if (!strcmp(opcode->value, "ble"))
  {
    encoded->cond = 0xd;
  }
  if (!strcmp(opcode->value, "bal"))
  {
    encoded->cond = 0xe;
  }
  if (!strcmp(opcode->value, "b"))
  {
    encoded->cond = 0xe;
  }

  return encoded;
}
