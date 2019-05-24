#include "Emulator.h"

void emulator_init(Emulator* emulator)
{
  int i;
  for (i = 0; i < REGISTER_COUNT; i++)
  {
    emulator->registers[i] = 0;
  }

  for (i = 0; i < MEMORY_SIZE; i++)
  {
    emulator->memory[i] = 0;
  }
}

void emulate(Emulator* emulator, char* src_file)
{
}

uint8_t get_mem_byte(Emulator* emulator, uint8_t addr)
{
  return emulator->memory[addr];
}

void set_mem_byte(Emulator* emulator, uint8_t addr, uint32_t val)
{
  emulator->memory[addr] = val;
}

uint8_t get_reg_bit(Emulator* emulator, uint8_t reg, uint8_t bit)
{
  return (emulator->registers[reg] >> bit) & 0x0001;
}

void set_reg_bit(Emulator* emulator, uint8_t reg, uint8_t bit)
{
  emulator->registers[reg] |= 1 << bit;
}

void clr_reg_bit(Emulator* emulator, uint8_t reg, uint8_t bit)
{
  emulator->registers[reg] &= ~(1 << bit);
}


/* Functions for setting flag bits */

uint8_t get_flag_N(Emulator* emulator)
{
  return get_reg_bit(emulator, REG_CPSR, FLAG_BIT_N);
}

void set_flag_N(Emulator* emulator)
{
  set_reg_bit(emulator, REG_CPSR, FLAG_BIT_N);
}

void clr_flag_N(Emulator* emulator)
{
  clr_reg_bit(emulator, REG_CPSR, FLAG_BIT_N);
}

uint8_t get_flag_Z(Emulator* emulator)
{
  return get_reg_bit(emulator, REG_CPSR, FLAG_BIT_Z);
}

void set_flag_Z(Emulator* emulator)
{
  set_reg_bit(emulator, REG_CPSR, FLAG_BIT_Z);
}

void clr_flag_Z(Emulator* emulator)
{
  clr_reg_bit(emulator, REG_CPSR, FLAG_BIT_Z);
}

uint8_t get_flag_C(Emulator* emulator)
{
  return get_reg_bit(emulator, REG_CPSR, FLAG_BIT_C);
}

void set_flag_C(Emulator* emulator)
{
  set_reg_bit(emulator, REG_CPSR, FLAG_BIT_C);
}

void clr_flag_C(Emulator* emulator)
{
  clr_reg_bit(emulator, REG_CPSR, FLAG_BIT_C);
}

uint8_t get_flag_V(Emulator* emulator)
{
  return get_reg_bit(emulator, REG_CPSR, FLAG_BIT_V);
}

void set_flag_V(Emulator* emulator)
{
  set_reg_bit(emulator, REG_CPSR, FLAG_BIT_V);
}

void clr_flag_V(Emulator* emulator)
{
  clr_reg_bit(emulator, REG_CPSR, FLAG_BIT_V);
}
