#ifndef _H_SYMB
#define _H_SYMB
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

Elf32_Sym *lecture_symb(FILE *felf,Elf32_Shdr *tab_section,Elf32_Ehdr elf_head,int *nbsymb,int *index);
void affiche_symbole_table (Elf32_Sym *symbtab,int nbsymb,int index,Elf32_Shdr *tab_section,FILE *felf);
#endif
