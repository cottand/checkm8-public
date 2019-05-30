#ifndef ASSEMBLER_H_
#define ASSEMBLER_H

#include <stdint.h>

typedef struct s_Assembler
{
    uint8_t *bin;
    char *src;
    uint16_t current_writing_bin_address;
} Assembler;

void assemble(char *src_program_file, char *dest_bin_file);

void load_from_src(Assembler *assembler, char *src_program_file);

uint8_t load_to_bin(Assembler *asse, char *bin_path);


uint8_t get_bin_byte(Assembler *assembler, uint16_t addr);
void set_bin_byte(Assembler *assembler, uint16_t addr, uint8_t val);

uint32_t get_bin_instr(Assembler *assembler, uint16_t addr);
void set_bin_instr(Assembler *assembler, uint16_t addr, uint32_t val);

#endif /* ASSEMBLER_H_ */