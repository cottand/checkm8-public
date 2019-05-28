#ifndef SHIFT_H_
#define SHIFT_H_

#include <stdint.h>

/* Logical shift left */
uint32_t lsl(uint32_t value, uint32_t shift, int *carry);

/* Logical shift right */
uint32_t lsr(uint32_t value, uint32_t shift, int *carry);

/* Arithmetic shift right */
uint32_t asr(uint32_t value, uint32_t shift, int *carry);

/* Rotate right */
uint32_t ror(uint32_t value, uint32_t shift, int *carry);

#endif /* SHIFT_H_ */
