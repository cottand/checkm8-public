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
  //encoded->i = 0x0;
  //encoded->p = 0x0;
  //encoded->u = 0x0;
  //encoded->l = 0x0;
  //encoded->rn = 0x0;
  encoded->rd = strtoul(rd->value, 0, 10);
  //encoded->offset = 0x0;
  /* TODO */

  return encoded;
}
