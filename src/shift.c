#include "shift.h"
#include <stdio.h>

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
