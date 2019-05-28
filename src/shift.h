#ifndef SHIFT_H_
#define SHIFT_H_

#include <stdint.h>

/* Logical shift left */
uint32_t lsl(uint32_t value, uint32_t shift);

/* Logical shift right */
uint32_t lsr(uint32_t value, uint32_t shift);

/* Arithmetic shift right */
uint32_t asr(uint32_t value, uint32_t shift);

/* Rotate right */
uint32_t ror(uint32_t value, uint32_t shift);

#endif /* SHIFT_H_ */
