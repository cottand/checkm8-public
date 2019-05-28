#include "execute.h"
#include <stdio.h>

void exec_instr(Emulator *emulator, Decoded_Instr *instr)
{
  switch (instr->type)
  {
  case Data_Proc:
    exec_data_proc_instr(emulator, instr->data_proc_instr);
    break;
  case Mul:
    exec_mul_instr(emulator, instr->mul_instr);
    break;
  case Data_Trans:
    exec_data_trans_instr(emulator, instr->data_trans_instr);
    break;
  case Branch:
    exec_branch_instr(emulator, instr->branch_instr);
    break;
  case Halt:
     emulator->halt = 1; 
  break;
  }
}

void exec_data_proc_instr(Emulator *emulator, Data_Proc_Instr *instr)
{
  if (!is_cond_true(emulator, instr->cond)) { return; }
}

void exec_mul_instr(Emulator *emulator, Mul_Instr *instr)
{
  if (!is_cond_true(emulator, instr->cond)) { return; }

  uint32_t res = 0;
  uint32_t rn  = emulator->regs[instr->rn];
  uint32_t rs  = emulator->regs[instr->rs];
  uint32_t rm  = emulator->regs[instr->rm];
  if (instr->a)
  {
    res = rm * rs + rn;
  }
  else
  {
    res = rm * rs;
  }

  emulator->regs[instr->rd] = res;

  if (instr->s)
  {
    if (!res)
    {
      set_flag_Z(emulator);
    }
    else
    {
      clr_flag_Z(emulator);
    }

    if (get_reg_bit(emulator, instr->rd, 31))
    {
      set_flag_N(emulator);
    }
    else
    {
      clr_flag_N(emulator);
    }
  }
}

void exec_data_trans_instr(Emulator *emulator, Data_Trans_Instr *instr)
{
  if (!is_cond_true(emulator, instr->cond)) { return; }

  uint32_t offset = 0;
  if (instr->i) /* Offset is shifted register */
  {
    /* The register is obtained with the last 4 bits */
    uint8_t reg = instr->offset & 0x0f;
    uint32_t reg_contents = emulator->regs[reg];

    /* Shift type is bits 5 and 6 of offset */
    uint8_t shift_type    = (instr->offset >> 5) & 0x3;
    /* Bit 4 determines whether the shift amount is in a reg or is a const */
    uint8_t use_shift_reg = (instr->offset >> 4) & 0x1;

    uint8_t shift_amount = 0;
    if (use_shift_reg)
    {
      uint8_t shift_reg = (instr->offset >> 8) & 0xf;

      /* Shit amount is last byte of shift reg */
      shift_amount = emulator->regs[shift_reg];
    }
    else
    {
      /* Shift amount is bits 7-11 of offset instr offset field */
      shift_amount = (instr->offset >> 7) & 0x1f;
    }

    switch (shift_type)
    {
    case 0x0: /* Logical left */
      offset = reg_contents << shift_amount;
      break;
    case 0x1: /* Logical right */
      offset = reg_contents >> shift_amount;
      break;
    case 0x2: /* Arithmetic right */
      if (reg_contents >> 31) /* if the number is negative */
      {
        offset = ~(~reg_contents >> shift_amount);
      }
      else
      {
        offset = reg_contents >> shift_amount;
      }
      break;
    case 0x3: /* Rotate right */
      if (!(shift_amount &= 31))
      {
        offset = reg_contents;
      }
      else
      {
        offset = (reg_contents >> shift_amount) | (reg_contents << (32 - shift_amount));
      }
      break;
    default:
      printf("Error: invalid shift type for data trans instr type");
      return;
    }
  }
  else /* Offset is unsigned 12 bit immediate offset */
  {
    offset = instr->offset;
  }

  uint32_t base = emulator->regs[instr->rn];
  uint32_t dst  = emulator->regs[instr->rd];

  if (instr->l) /* Load from memory */
  {
    base = emulator->mem[dst];
  }
  else /* Store to memory */
  {
    emulator->mem[dst] = base;
  }

  /* Only support post indexing */
  if (!instr->p)
  {
    base = instr-> u ? base + offset : base - offset;
  }

  /* Save the new values into regs */
  emulator->regs[instr->rn] = base;
  emulator->regs[instr->rd] = dst;
}

void exec_branch_instr(Emulator *emulator, Branch_Instr *instr)
{
  if (!is_cond_true(emulator, instr->cond)) { return; }

  uint32_t offset = instr->offset << 2;
  uint32_t pc = get_PC(emulator);
  pc += offset;

  set_PC(emulator, pc);
}

uint8_t is_cond_true(Emulator *emulator, uint8_t cond)
{
  switch (cond)
  {
  case 0x0:
    return (get_flag_Z(emulator));
  case 0x1:
    return (!get_flag_Z(emulator));
  case 0xa:
    return (get_flag_V(emulator) == get_flag_N(emulator));
  case 0xb:
    return (get_flag_V(emulator) != get_flag_N(emulator));
  case 0xc:
    return (!get_flag_Z(emulator)
        && (get_flag_V(emulator) == get_flag_N(emulator)));
  case 0xd:
    return (get_flag_Z(emulator)
        || (get_flag_V(emulator) != get_flag_N(emulator)));
  case 0xe:
    return 1;
  default:
    printf("Error: invalid instruction condition code %x", cond);
    return 0;
  }
}
