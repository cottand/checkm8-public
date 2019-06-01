#include "encode_data_proc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Data_Proc_Instr *encode_data_proc_instr(Token_Stream *instr)
{
  Data_Proc_Instr *encoded = malloc(sizeof(Data_Proc_Instr));

  Token *opcode = token_stream_expect(instr, Opcode, "Expecting opecode for data_proc type instr");
  char *v = opcode->value;

  encoded->cond = 0xe;
  encoded->s    = 0x0;

  if (!strcmp(v, "and") || !strcmp(v, "eor") ||
      !strcmp(v, "sub") || !strcmp(v, "rsb") ||
      !strcmp(v, "add") || !strcmp(v, "orr"))
  {
    encode_compute(encoded, instr);
  }

  if (!strcmp(v, "mov"))
  {
    encode_mov(encoded, instr);
  }

  if (!strcmp(v, "tst") || !strcmp(v, "teq") || !strcmp(v, "cmp"))
  {
    encode_test(encoded, instr);

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

  uint8_t val8 = (val32 >> rightmost_bit) & 0xff;

  /* By how much we have to rotate right to put the 8 bits back where they were */
  uint8_t rotate = 0x0;
  if (leftmost_bit > 8)
  {
    rotate = 32 - rightmost_bit;
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

    uint8_t shift_type = encode_operand2_shift(shift->value);

    /* Surround shift_type with 0 left and 1 right (as in spec) */
    shift_type = (shift_type << 1) | 0x1;

    shift_enc = (rs_enc << 4) | shift_type;
  }

  if (peak->symb == Immediate)
  {
    Token *imm = token_stream_read(tokens);

    uint8_t shift_type  = encode_operand2_shift(shift->value);
    uint8_t imm_enc     = encode_immediate(imm->value);

    shift_enc = (imm_enc << 3) | (shift_type << 1);
  }

  instr->operand_2 = (shift_enc << 4) | rm_enc;
}

uint8_t encode_operand2_shift(char *shift)
{
  if (!strcmp(shift, "lsl"))
  {
    return 0x0;
  }
  if (!strcmp(shift, "lsr"))
  {
    return 0x1;
  }
  if (!strcmp(shift, "asr"))
  {
    return 0x2;
  }
  if (!strcmp(shift, "ror"))
  {
    return 0x3;
  }

  return 0x0;
}

uint32_t encode_immediate(char *immediate)
{
  /* Convert from hex or decimal */
  if (immediate[1] == 'x')
  {
    return strtoul(immediate, 0, 16);
  }
  else
  {
    return strtoul(immediate, 0, 10);
  }
}
