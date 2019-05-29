#include "emulator.h"

#include "loader.h"
#include "pipeline.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void emulator_init(Emulator *emulator)
{
  int i;
  emulator->halt = 0;
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
  load_binary(emulator, src_file);

  Pipeline pipeline;
  pipeline_init(&pipeline, emulator);

  while (!emulator->halt)
  {
    cycle_p(&pipeline);
  }
  /**
   *  We don't increment PC on the very last "halt" instruction
   * but pipeline increases it at every cycle, so we're decreasing
   * it at the end of runtime 
   */
  set_PC(emulator, get_PC(emulator) - sizeof(uint32_t));
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

/**
 * Returns the instruction at the address in memory stored in PC
 */
uint32_t fetch(Emulator *emulator)
{
  uint16_t pc_addr = (uint16_t)get_PC(emulator);
  /*uint8_t i_bits[4];
  for(int i = 0; i < 4; i++)
  {
    i_bits[i] = get_mem_byte(emulator, pc_addr+(3-i));
  }
  uint32_t *instr = (uint32_t *) &i_bits[0];
  */
  uint32_t instr;
  uint8_t *memory_byte = &(emulator->mem[pc_addr]);
  memcpy(&instr, memory_byte, 4);
  return instr;
}

void set_PC_addr(Emulator *emulator, uint16_t addr)
{
  /* This cast should be safe */
  set_PC(emulator, (uint32_t)addr);
}

/* Functions for printing */

void print_regs(Emulator *emulator)
{
  printf("Registers:\n");

  int i;
  for (i = 0; i < REG_COUNT; i++)
  {
    if (i < 13)
    {
      printf("$%-3d:\t%3d (0x%08x)\n", i, emulator->regs[i], emulator->regs[i]);
    }
    else if (i != 13 && i != 14)
    {
      char *name = malloc(4);
      switch (i)
      {
      /*case 13:
        strcpy(name, "SP");
        break;
      case 14:
        strcpy(name, "LR");
        break;*/
      case 15:
        strcpy(name, "PC");
        break;
      case 16:
        strcpy(name, "CPSR");
        break;
      }

      printf("%-4s:\t%3d (0x%08x)\n", name, emulator->regs[i], emulator->regs[i]);
      free(name);
    }
  }

  /*uint8_t n = get_flag_N(emulator);
  uint8_t z = get_flag_Z(emulator);
  uint8_t c = get_flag_C(emulator);
  uint8_t v = get_flag_V(emulator);
  printf("N:%d | Z:%d | C:%d | V:%d\n", n, z, c, v);*/
}

void print_mem(Emulator *emulator)
{
  printf("Non-zero memory:\n");

  int i;
  for (i = 0; i < MEM_SIZE-4; i += 4)
  {
    uint32_t memory_content;
    uint8_t *memory_array = malloc(4 * sizeof(uint8_t*));
    int j, k;
    k = 0;
    for(j = 3; j >= 0; j--)
    {
      memory_array[k] = emulator->mem[i+j];
      k++;
    }
    memcpy(&memory_content, memory_array, 4);
    if (emulator->mem[i] != 0)
    {
      printf("0x%08x: 0x%08x\n", i, memory_content);
    }
  }
}
