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

uint8_t *read_src(char *src_program_file)
{
    //TODO
}

uint8_t *open_bin(char *dest_bin_file)
{
    //TODO
}

int close_bin(char *src_program_file, uint8_t *src_array)
{
    //TODO
}

uint8_t get_src_byte(Assembler *assembler, uint16_t addr)
{
}
void set_src_byte(Assembler *assembler, uint16_t addr, uint8_t val)
{
}

uint8_t get_bin_byte(Assembler *assembler, uint16_t addr)
{
}
void set_bin_byte(Assembler *assembler, uint16_t addr, uint8_t val)
{
}

uint32_t get_bin_instr(Assembler *assembler, uint16_t addr)
{
}
void set_bin_instr(Assembler *assembler, uint16_t addr, uint32_t val)
{
}
