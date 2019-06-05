#ifndef LOADER_H_
#define LOADER_H_
#include <stdint.h>

void load(char* path, void *ptr);

uint32_t get_filesize(char* path);

#endif /* LOADER_H_ */
