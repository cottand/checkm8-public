#include "emulator.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "loader.h"

void emulator_init(Emulator *emulator)
{
  int i;
  for (i = 0; i < REG_COUNT; i++)
  {
    emulator->regs[i] = 0;
  }

  for (i = 0; i < MEM_SIZE; i++)
  {
    emulator->mem[i] = 0;
  }
}

void emulate(Emulator *emulator, char *src_file)
{
}

void load_binary(Emulator *emulator, char *src_file)
{
  load(src_file, &(emulator->mem));
}

uint8_t get_mem_byte(Emulator *emulator, uint16_t addr)
{
  return emulator->mem[addr];
}

void set_mem_byte(Emulator *emulator, uint16_t addr, uint8_t val)
{
  emulator->mem[addr] = val;
}

uint8_t get_reg_bit(Emulator *emulator, uint8_t reg, uint8_t bit)
{
  return (emulator->regs[reg] >> bit) & 0x0001;
}

void set_reg_bit(Emulator *emulator, uint8_t reg, uint8_t bit)
{
  emulator->regs[reg] |= 1 << bit;
}

void clr_reg_bit(Emulator *emulator, uint8_t reg, uint8_t bit)
{
  emulator->regs[reg] &= ~(1 << bit);
}

uint32_t get_PC(Emulator *emulator)
{
  return emulator->regs[REG_PC];
}

void set_PC(Emulator *emulator, uint32_t val)
{
  emulator->regs[REG_PC] = val;
}

void incr_PC(Emulator *emulator)
{
  emulator->regs[REG_PC] += sizeof(uint32_t);
}

/* Functions for setting flag bits */

uint8_t get_flag_N(Emulator *emulator)
{
  return get_reg_bit(emulator, REG_CPSR, FLAG_BIT_N);
}

void set_flag_N(Emulator *emulator)
{
  set_reg_bit(emulator, REG_CPSR, FLAG_BIT_N);
}

void clr_flag_N(Emulator *emulator)
{
  clr_reg_bit(emulator, REG_CPSR, FLAG_BIT_N);
}

uint8_t get_flag_Z(Emulator *emulator)
{
  return get_reg_bit(emulator, REG_CPSR, FLAG_BIT_Z);
}

void set_flag_Z(Emulator *emulator)
{
  set_reg_bit(emulator, REG_CPSR, FLAG_BIT_Z);
}

void clr_flag_Z(Emulator *emulator)
{
  clr_reg_bit(emulator, REG_CPSR, FLAG_BIT_Z);
}

uint8_t get_flag_C(Emulator *emulator)
{
  return get_reg_bit(emulator, REG_CPSR, FLAG_BIT_C);
}

void set_flag_C(Emulator *emulator)
{
  set_reg_bit(emulator, REG_CPSR, FLAG_BIT_C);
}

void clr_flag_C(Emulator *emulator)
{
  clr_reg_bit(emulator, REG_CPSR, FLAG_BIT_C);
}

uint8_t get_flag_V(Emulator *emulator)
{
  return get_reg_bit(emulator, REG_CPSR, FLAG_BIT_V);
}

void set_flag_V(Emulator *emulator)
{
  set_reg_bit(emulator, REG_CPSR, FLAG_BIT_V);
}

void clr_flag_V(Emulator *emulator)
{
  clr_reg_bit(emulator, REG_CPSR, FLAG_BIT_V);
}

uint16_t get_PC_addr(Emulator *emulator)
{
  uint32_t PC_content = get_PC(emulator);
  /*
  It would be nice if we could find a way of checking
  PC_content <= UINT16_MAX
  */
  return (uint16_t) PC_content;
}

void set_PC_addr(Emulator *emulator, uint16_t addr)
{
  set_PC(emulator, (uint32_t) addr);
}

uint16_t fetch_instr(Emulator *emulator)
{
  uint16_t to_ret = get_PC_addr(emulator);
  incr_PC(emulator);
  return to_ret;
}

/* Functions for printing */

void print_regs(Emulator *emulator)
{
  printf("Reg\t\tValue\n");

  int i;
  for (i = 0; i < REG_COUNT; i++)
  {
    if (i < 13)
    {
      printf("[%d]\t=\t0x%x\n", i, emulator->regs[i]);
    }
    else
    {
      char *name = malloc(4);
      switch (i)
      {
      case 13:
        strcpy(name, "SP");
        break;
      case 14:
        strcpy(name, "LR");
        break;
      case 15:
        strcpy(name, "PC");
        break;
      case 16:
        strcpy(name, "CPSR");
        break;
      }

      printf("[%s]\t=\t0x%x\n", name, emulator->regs[i]);
      free(name);
    }
  }

  uint8_t n = get_flag_N(emulator);
  uint8_t z = get_flag_Z(emulator);
  uint8_t c = get_flag_C(emulator);
  uint8_t v = get_flag_V(emulator);
  printf("N:%d | Z:%d | C:%d | V:%d\n", n, z, c, v);
}

void print_mem(Emulator *emulator)
{
  printf("Addr\t\tValue\n");

  short skipping = 0;

  int i;
  for (i = 0; i < MEM_SIZE; i++)
  {
    if (emulator->mem[i] == 0)
    {
      if (!skipping)
      {
        skipping = 1;
        printf("...\t\t0x0\n");
      }
    }
    else
    {
      skipping = 0;
      printf("[0x%x]\t=\t0x%x\n", i, emulator->mem[i]);
    }
  }
}
