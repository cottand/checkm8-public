#include "assembler.h"
#include <stdio.h>
#include <string.h>

#include "loader.h"

void assemble(char *src_program_file, const char *dest_bin_file)
{
    Assembler asse;
    asse.src = read_src(src_program_file);
    strcpy(&(asse.bin_path), dest_bin_file);
    open_bin(&asse);

    //TODO

    free(asse.src);
    if (close_bin(dest_bin_file != 0))
    {
        perror("Save to bin file unsuccsessful");
    }
}

void load_from_src(Assembler *assembler, char *src_program_file)
{
    load(src_program_file, &(assembler->src));
}
uint8_t open_bin(Assembler *asse)
{
    if ((asse->bin_file = fopen(asse->bin_path, 'w')) == NULL)
    {
        perror("Error opening/creating bin file");
    }
    asse->current_bin_w_address = 0;
}

/**
 * Takes the path to the already opened bin file and writes 
 * the array at bin_array to it. Returns 1 if everything went ok
 */
uint8_t close_bin(Assembler *asse)
{
    return (uint8_t)fclose(asse->bin_file);
}

void set_bin_instr(Assembler *assembler, uint16_t addr, uint32_t val)
{
    perror("Not implemented: set_bin_instr");
}

void set_next_bin_instr(Assembler *assembler, uint32_t *instr)
{
    /*
     set_bin_instr(assembler, assembler->current_writing_bin_address, val);
    assembler->current_writing_bin_address += sizeof(uint32_t);
    */
    const void *from_addr = instr;
    fwrite(from_addr, sizeof(uint32_t), 1, assembler->bin_file);
    assembler->current_bin_w_address += sizeof(uint32_t);
}

uint16_t get_last_written_address_bin(Assembler *assembler)
{
    return assembler->current_bin_w_address - sizeof(uint32_t);
}