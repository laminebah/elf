#ifndef __PRINT_SYMBOLE__
#define __PRINT_SYMBOLE__

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>

void print_type(Elf32_Sym *sym);

void print_link(Elf32_Sym *sym);

void print_visi(Elf32_Sym *sym);

#endif

