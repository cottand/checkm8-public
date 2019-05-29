#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#define BIN_SIZE 1 //TO FIX
#define PROG_SIZE 1

#include <stdint.h>

typedef struct s_Assembler
{
    uint8_t *bin[BIN_SIZE];
    uint8_t *src[PROG_SIZE];
} Assembler;

void assemble(char *src_program_file, char *dest_bin_file);

uint8_t get_src_byte(Assembler *assembler, uint16_t addr);
void set_src_byte(Assembler *assembler, uint16_t addr, uint8_t val);

uint8_t get_bin_byte(Assembler *assembler, uint16_t addr);
void set_bin_byte(Assembler *assembler, uint16_t addr, uint8_t val);

uint32_t get_bin_instr(Assembler *assembler, uint16_t addr);
void set_bin_instr(Assembler *assembler, uint16_t addr, uint32_t val);

#endif /* ASSEMBLER_H_ */
