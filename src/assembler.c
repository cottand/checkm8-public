#include <string.h>
#include <stdlib.h>
#include "loader.h"
#include "assembler.h"


void load_from_src(Assembler *assembler, char *src_program_file)
{
    //load(src_program_file, &(assembler->src));
    load(src_program_file, (void *) assembler->src);
    //wtf why is this not linking pls halp
}

uint8_t open_bin(Assembler *asse)
{
    if ((asse->bin_file = fopen(asse->bin_path,
                                (const char *restrict) & "w")) == NULL)
    {
        perror("Error opening/creating bin file");
        return -1;
    }
    asse->current_bin_w_address = 0;
    return 0;
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

void assembler_init(Assembler *assembler, char *src_program_file,
                    char const *dest_bin_file)
{
    load_from_src(assembler, src_program_file);
    strcpy((char *restrict) & (assembler->bin_path), dest_bin_file);
    open_bin(assembler);
    assembler->src = malloc(get_filesize(src_program_file));
}
void assemble(char *src_program_file, const char *dest_bin_file)
{
    Assembler asse;
    assembler_init(&asse, src_program_file, dest_bin_file);

    //TODO

    free(asse.src);
    if (close_bin(&asse) != 0)
    {
        perror("Save to bin file unsuccsessful");
    }
}