#include "assembler.h"

#include "loader.h"

void assemble(char *src_program_file, char *dest_bin_file)
{
    /**
     * TODO:
     * load both files into arrays, src as read_only,
     * Then assign those arrays to the struct Assembler
     */
}

uint8_t get_src_byte(Assembler *assembler, uint16_t addr)
{
    return -1;
}
void set_src_byte(Assembler *assembler, uint16_t addr, uint8_t val)
{
}

uint8_t get_bin_byte(Assembler *assembler, uint16_t addr)
{
    return -1;
}

void set_bin_byte(Assembler *assembler, uint16_t addr, uint8_t val)
{
}

uint32_t get_bin_instr(Assembler *assembler, uint16_t addr)
{
    return -1;
}

void set_bin_instr(Assembler *assembler, uint16_t addr, uint32_t val)
{
}
