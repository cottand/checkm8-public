#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stdint.h>
#include <stdio.h>
#include "instruction.h"

typedef struct s_Assembler
{
    char *src;
    char bin_path[100];
    FILE *bin_file;
    uint16_t current_bin_w_address;
} Assembler;

void assemble(char *src_program_file, const char *dest_bin_file);

void load_from_src(Assembler *assembler, char *src_program_file);

void set_next_bin_instr(Assembler *assembler, uint32_t *instr);

uint16_t get_last_written_address_bin(Assembler *assembler);
#endif /* ASSEMBLER_H_ */