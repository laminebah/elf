#ifndef _H_ELF_HEADER_
#define _H_ELF_HEADER_
#include <elf.h>
#include <stdio.h>
#include "util.h"
int ElfId (Elf32_Ehdr * elf_head);
Elf32_Ehdr * lecture_entete (FILE *felf);


//convert little endian
int32_t value_32(int32_t value);
int16_t value_16(int16_t value);

#endif 
