#ifndef EMULATOR_H_
#define EMULATOR_H_

#include <stdint.h>

#define REG_COUNT 17
#define MEM_SIZE  65536

#define REG_SP    13
#define REG_LR    14
#define REG_PC    15
#define REG_CPSR  16

#define FLAG_BIT_N 31
#define FLAG_BIT_Z 30
#define FLAG_BIT_C 29
#define FLAG_BIT_V 28

typedef struct s_Emulator
{
  uint32_t regs[REG_COUNT];
  uint32_t mem[MEM_SIZE];
} Emulator;


void emulator_init(Emulator* emulator);
void emulate(Emulator* emulator, char* src_file);

uint8_t get_mem_byte(Emulator* emulator, uint8_t addr);
void    set_mem_byte(Emulator* emulator, uint8_t addr, uint32_t val);

uint8_t get_reg_bit(Emulator* emulator, uint8_t reg, uint8_t bit);
void    set_reg_bit(Emulator* emulator, uint8_t reg, uint8_t bit);
void    clr_reg_bit(Emulator* emulator, uint8_t reg, uint8_t bit);

uint8_t get_flag_N(Emulator* emulator);
void    set_flag_N(Emulator* emulator);
void    clr_flag_N(Emulator* emulator);
uint8_t get_flag_Z(Emulator* emulator);
void    set_flag_Z(Emulator* emulator);
void    clr_flag_Z(Emulator* emulator);
uint8_t get_flag_C(Emulator* emulator);
void    set_flag_C(Emulator* emulator);
void    clr_flag_C(Emulator* emulator);
uint8_t get_flag_V(Emulator* emulator);
void    set_flag_V(Emulator* emulator);
void    clr_flag_V(Emulator* emulator);

void print_regs(Emulator* emulator);
void print_mem(Emulator* emulator);

#endif /* EMULATOR_H_ */
