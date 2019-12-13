#ifndef _H_ELF_HEADER_
#define _H_ELF_HEADER_
#include <elf.h>
#include <stdio.h>
int ElfId (FILE *felf,Elf32_Ehdr * elf_head);
Elf32_Ehdr *lecture_entete (FILE *felf);
#endif 
