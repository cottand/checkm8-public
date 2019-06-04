#include "shift.h"
#include <stdio.h>
#include <string.h>

uint32_t lsl(uint32_t value, uint32_t shift, uint32_t *carry)
{
  *carry = shift > 0 ? (value >> (32 - shift)) & 1 : 0;
  return value << shift;
}

uint32_t lsr(uint32_t value, uint32_t shift, uint32_t *carry)
{
  *carry = (value >> shift) & 1;
  return value >> shift;
}

uint32_t asr(uint32_t value, uint32_t shift, uint32_t *carry)
{
  *carry = (value >> (32 - shift)) & 1;
  /* If the number is negative*/
  if (value >> 31)
  {
    return ~(~value >> shift);
  }
  else
  {
    return value >> shift;
  }
}

uint32_t ror(uint32_t value, uint32_t shift, uint32_t *carry)
{
  *carry = (value >> shift) & 1;
  return (value >> shift) | (value << (32 - shift));
}

uint8_t encode_shift_opcode(char *shift)
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
