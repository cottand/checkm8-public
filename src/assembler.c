#include "assembler.h"
#include <stdio.h>
#include <string.h>

#include "loader.h"

void assemble(char *src_program_file, char *dest_bin_file)
{
    Assembler asse;
    asse.src = read_src(src_program_file);

    /**
     * TODO:
     * load both files into arrays, src as read_only,
     * Then assign those arrays to the struct Assembler
     */

    free(asse.src);
    if (close_bin(dest_bin_file, *(asse.bin)) != 1)
    {
        printf("Save to bin unsuccsessful");
    }
}

char **read_src(char *src_program_file)
{
    //TODO
}


/**
 * Takes the path to the already opened bin file and writes 
 * the array at bin_array to it. Returns 1 if everything went ok
 */
uint8_t load_to_bin(Assembler *asse, char *bin_path)
{
    //TODO
    return 1;
}

uint8_t get_bin_byte(Assembler *assembler, uint16_t addr)
{
    return 0;
}
void set_bin_byte(Assembler *assembler, uint16_t addr, uint8_t val)
{
}

uint32_t get_bin_instr(Assembler *assembler, uint16_t addr)
{
    return 0;
}

void set_bin_instr(Assembler *assembler, uint16_t addr, uint32_t val)
{
}

void set_next_bin_instr(Assembler *assembler, uint32_t val){
    set_bin_instr(assembler, assembler->current_writing_bin_address, val);
    assembler->current_writing_bin_address += sizeof(uint32_t);
}
