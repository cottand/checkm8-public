#include "encode_data_trans.h"
#include <stdlib.h>

/*
 * PRE: <address> is not a numeric constant
 */
Data_Trans_Instr *encode_data_trans_instr(Token_Stream *instr)
{
  Data_Trans_Instr *encoded = malloc(sizeof(Data_Trans_Instr));

  Token *opcode = token_stream_expect(instr, Opcode, "Expecting opcode for data_trans type instr (ldr, str)");

  Token *rd = token_stream_expect(instr, Register, "Expecting Rd register for data_trans type instruction (ldr, str)");

  /* TODO: Get <address> argument into address variable */
  Token *address;

  encoded->cond = 0xe;
  encoded->rd = strtoul(rd->value, 0, 10);

  /* TODO:
   * encoded->i
   * encoded->p
   * encoded->u
   * encoded->rn
   * encoded->offset 
   */

  if (!strcmp(opcode->value, "ldr"))
  {
    encoded->l = 0x1;
  }
  if (!strcmp(opcode->value, "str"))
  {
    encoded->l = 0x0;
  }

  return encoded;
}
