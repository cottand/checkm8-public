#include "encode_data_trans.h"
#include <stdlib.h>
#include <string.h>
#include "shift.h"
#include "encode.h"

#include <stdio.h>

/*
 * PRE: <address> is not a numeric constant.
 * Hence, <address> is of the form [<...>].
 */
Data_Trans_Instr *encode_data_trans_instr(Token_Stream *instr)
{
  Data_Trans_Instr *encoded = malloc(sizeof(Data_Trans_Instr));

  Token *opcode = token_stream_expect(instr, Opcode, "Expecting opcode for data_trans type instr (ldr, str)");
  Token *rd = token_stream_expect(instr, Register, "Expecting Rd register for data_trans type instr (ldr, str)");
  token_stream_expect(instr, LBracket, "Expecting Left Bracket for data_trans type instr (ldr, str)");
  Token *rn = token_stream_expect(instr, Register, "Expecting Rn register for data_trans type instr (ldr, str)");

  //Check if we are post-indexing or pre-indexing.
  Token *next = token_stream_peak(instr);

  if (next->symb == RBracket)
  {
    if (!(next->next))
    {
      // Pre-Indexing with offset 0
      encode_data_trans_instr_pre_indexing(encoded, instr, next->next);
    }
    else
    {
      // Post-Indexing
      encode_data_trans_instr_post_indexing(encoded, instr, next->next);
    }
  }
  else
  {
    // Pre-Indexing
    encode_data_trans_instr_pre_indexing(encoded, instr, next);
  }

  encoded->cond = 0xe;
  encoded->rd = strtoul(rd->value, 0, 10);
  encoded->rn = strtoul(rn->value, 0, 10);

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

void encode_data_trans_instr_pre_indexing(Data_Trans_Instr *encoded, Token_Stream *instr, Token *next)
{
  encoded->p = 0x1;
  if (!next)
  {
    // <address> = <rn> with immediate offset of +0.
    encoded->i = 0x0;
    encoded->u = 0x1;
    encoded->offset = 0x0;
    return;
  }
  if (next->symb == Immediate)
  {
    // <address> = <rn> with immediate offset of +<offset>
    encoded->i = 0x0;
    encoded->u = !(next->value[0] == '-');
    if (!encoded->u)
    {
      encoded->offset = encode_immediate(&next->value[1]);
    }
    else
    {
      encoded->offset = encode_immediate(next->value);
    }
  }
  if (next->symb == Register)
  {
    // <address> = <rn> with offset of + a shifted register
    encoded->i = 0x1;
    encoded->u = 0x1;
    encode_data_trans_instr_shifted_register(encoded, instr);
  }
  if (next->symb == Address)
  {
    encoded->u = 0x1;
    encoded->i = 0x0;
    encoded->offset = strtoul(next->value, 0, 16);
  }
}

void encode_data_trans_instr_post_indexing(Data_Trans_Instr *encoded, Token_Stream *instr, Token *next)
{
  encoded->p = 0x0;

  if (next->symb == Immediate)
  {
    encoded->i = 0x0;
    encoded->u = 0x1;
    encoded->offset = strtoul(next->value, 0, 10);
  }
  if (next->symb == Register)
  {
    encoded->i = 0x1;
    encoded->u = 0x1;
    encode_data_trans_instr_shifted_register(encoded, instr);
  }
}

// Possibly duplicate of encode_operand2_register in encode_data_proc.c
void encode_data_trans_instr_shifted_register(Data_Trans_Instr *encoded, Token_Stream *instr)
{
  Token *rm = token_stream_expect(instr, Register, "Expecting Rm for shifted register in data_trans type instr");
  uint8_t rm_enc = strtoul(rm->value, 0, 10);

  Token *end_peek = token_stream_peak(instr);
  if (!end_peek)
  {
    encoded->offset = rm_enc;
    return;
  }
  if(end_peek->symb == RBracket)
  {
    encoded->offset = rm_enc;
    return;
  }

  Token *shift = token_stream_expect(instr, Opcode, "Expecting shift type for shifted register in data_trans type instr");
  uint8_t shift_enc = 0x0;

  Token *peek = token_stream_peak(instr);
  if (peek->symb == Register)
  {
    Token *rs = token_stream_read(instr);
    uint8_t rs_enc = strtoul(rs->value, 0, 10);

    uint8_t shift_type = encode_shift_opcode(shift->value);

    /* Surround shift_type with 0 left and 1 right (as in spec) */
    shift_type = (shift_type << 1) | 0x1;

    shift_enc = (rs_enc << 4) | shift_type;
  }

  if (peek->symb == Immediate)
  {
    Token *imm = token_stream_read(instr);

    uint8_t shift_type = encode_shift_opcode(shift->value);
    uint8_t imm_enc = encode_immediate(imm->value);

    shift_enc = (imm_enc << 3) | (shift_type << 1);
  }

  encoded->offset = (shift_enc << 4) | rm_enc;
}
