#ifndef SHIFT_H_
#define SHIFT_H_

#include <stdint.h>

/* Logical shift left */
uint32_t lsl(uint32_t value, uint32_t shift, uint32_t *carry);

/* Logical shift right */
uint32_t lsr(uint32_t value, uint32_t shift, uint32_t *carry);

/* Arithmetic shift right */
uint32_t asr(uint32_t value, uint32_t shift, uint32_t *carry);

/* Rotate right */
uint32_t ror(uint32_t value, uint32_t shift, uint32_t *carry);

/* Util used for Encode */
uint8_t encode_shift_opcode(char *shift);

#endif /* SHIFT_H_ */
