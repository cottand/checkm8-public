#include "encode_data_proc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shift.h"
#include "encode.h"

Data_Proc_Instr *encode_data_proc_instr(Token_Stream *instr)
{
  Data_Proc_Instr *encoded = malloc(sizeof(Data_Proc_Instr));

  Token *opcode = token_stream_expect(instr, Opcode, "Expecting opecode for data_proc type instr");
  char *v = opcode->value;

  encoded->cond = 0xe;
  encoded->s    = 0x0;

  /* Compute type instructions */
  if (!strcmp(v, "and"))
  {
    encode_compute(encoded, instr);
    encoded->opcode = 0x0;
  }
  if (!strcmp(v, "eor"))
  {
    encode_compute(encoded, instr);
    encoded->opcode = 0x1;
  }
  if (!strcmp(v, "sub"))
  {
    encode_compute(encoded, instr);
    encoded->opcode = 0x2;
  }
  if (!strcmp(v, "rsb"))
  {
    encode_compute(encoded, instr);
    encoded->opcode = 0x3;
  }
  if (!strcmp(v, "add"))
  {
    encode_compute(encoded, instr);
    encoded->opcode = 0x4;
  }
  if (!strcmp(v, "orr"))
  {
    encode_compute(encoded, instr);
    encoded->opcode = 0xc;
  }

  /* mov instruction */
  if (!strcmp(v, "mov"))
  {
    encode_mov(encoded, instr);
    encoded->opcode = 0xd;
  }

  /* lsl instruction */
  if (!strcmp(v, "lsl"))
  {
    encode_lsl(encoded, instr);
    encoded->opcode = 0xd;
  }

  /* Compare type instructions */
  if (!strcmp(v, "tst"))
  {
    encode_test(encoded, instr);
    encoded->opcode = 0x8;
    encoded->s = 0x1;
  }
  if (!strcmp(v, "teq"))
  {
    encode_test(encoded, instr);
    encoded->opcode = 0x9;
    encoded->s = 0x1;
  }
  if (!strcmp(v, "cmp"))
  {
    encode_test(encoded, instr);
    encoded->opcode = 0xa;
    encoded->s = 0x1;
  }

  encode_operand2(encoded, instr);

  return encoded;
}

void encode_compute(Data_Proc_Instr *instr, Token_Stream *tokens)
{
  Token *rd = token_stream_expect(tokens, Register, "Expecting Rd for data_proc instr");
  Token *rn = token_stream_expect(tokens, Register, "Expecting Rn for data_proc instr");

  instr->rd = strtoul(rd->value, 0, 10);
  instr->rn = strtoul(rn->value, 0, 10);
}

void encode_mov(Data_Proc_Instr *instr, Token_Stream *tokens)
{
  Token *rd = token_stream_expect(tokens, Register, "Expecting Rd for data_proc instr");

  instr->rd = strtoul(rd->value, 0, 10);
  instr->rn = 0x0;
}

void encode_lsl(Data_Proc_Instr *instr, Token_Stream *tokens)
{
  Token *rn = token_stream_expect(tokens, Register, "Expecting Rn for data_proc instr");

  instr->rd = 0x0;
  instr->rn = strtoul(rn->value, 0, 10);

  /* We transform lsl Rn <#expression> into mov Rn Rn lsl <#expression> */
  Token *rn2 = malloc(sizeof(Token));
  token_init(rn2);
  rn2->symb  = rn->symb;
  rn2->value = malloc(sizeof(char));
  strcpy(rn2->value, rn->value);

  Token *lsl = malloc(sizeof(Token));
  token_init(lsl);
  lsl->symb  = Opcode;
  lsl->value = malloc(sizeof(char));
  strcpy(lsl->value, "lsl");

  rn2->next = lsl;
  lsl->next = rn->next;
  rn->next  = rn2;
}

void encode_test(Data_Proc_Instr *instr, Token_Stream *tokens)
{
  Token *rn = token_stream_expect(tokens, Register, "Expecting Rn for data_proc instr");

  instr->rd = 0x0;
  instr->rn = strtoul(rn->value, 0, 10);
}

void encode_operand2(Data_Proc_Instr *instr, Token_Stream *tokens)
{
  if (token_stream_peak(tokens)->symb == Immediate)
  {
    instr->i = 0x1;
    encode_operand2_immediate(instr, tokens);
  }
  else
  {
    instr->i = 0x0;
    encode_operand2_register(instr, tokens);
  }
}

void encode_operand2_immediate(Data_Proc_Instr *instr, Token_Stream *tokens)
{
  Token *imm = token_stream_read(tokens);
  uint32_t val32 = encode_immediate(imm->value);

  /* We now make sure the value can be represented with 8 bits max */
  uint8_t leftmost_bit  = 32 - __builtin_clz(val32);
  uint8_t rightmost_bit = ffs(val32) - 1;

  /* Size in bits of the immediate */
  uint8_t size = leftmost_bit - rightmost_bit;

  if (size > 8)
  {
    printf("Error: immediate for operand2 can't be fitted into 8 bits: %x\n", val32);
    return;
  }

  uint8_t rotate = 0x0;
  uint8_t val8   = val32 & 0xff;

  /* If the number isn't in the rightmost 8 bits, we shift it here and compute
   * by how much we have to rotate right after to put it back where it was. */
  if (leftmost_bit > 8)
  {
    rotate = 32 - rightmost_bit;
    val8   = (val32 >> rightmost_bit) & 0xff;
  }

  if (rotate % 2 != 0 || rotate > 30)
  {
    printf("Error: rotate for immediate for operand2 can't be represented: %d\n", rotate);
  }

  rotate /= 2;
  instr->operand_2 = (rotate << 8) | val8;
}

void encode_operand2_register(Data_Proc_Instr *instr, Token_Stream *tokens)
{
  Token *rm = token_stream_expect(tokens, Register, "Expecting Rm for operand2");
  uint8_t rm_enc = strtoul(rm->value, 0, 10);

  if (!token_stream_peak(tokens))
  {
    instr->operand_2 = rm_enc;
    return;
  }

  Token *shift = token_stream_expect(tokens, Opcode, "Expecting shift type");
  uint8_t shift_enc = 0x0;

  Token *peak = token_stream_peak(tokens);
  if (peak->symb == Register)
  {
    Token *rs = token_stream_read(tokens);
    uint8_t rs_enc = strtoul(rs->value, 0, 10);

    uint8_t shift_type = encode_shift_opcode(shift->value);

    /* Surround shift_type with 0 left and 1 right (as in spec) */
    shift_type = (shift_type << 1) | 0x1;

    shift_enc = (rs_enc << 4) | shift_type;
  }

  if (peak->symb == Immediate)
  {
    Token *imm = token_stream_read(tokens);

    uint8_t shift_type  = encode_shift_opcode(shift->value);
    uint8_t imm_enc     = encode_immediate(imm->value);

    shift_enc = (imm_enc << 3) | (shift_type << 1);
  }

  instr->operand_2 = (shift_enc << 4) | rm_enc;
}
