#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <byteswap.h>
#include <assert.h>
#include "Elf_header.h"

#define SIZENAME 30

int32_t val_32(Elf32_Ehdr h, int32_t value);

int16_t val_16(Elf32_Ehdr h, int16_t value);

void read_section(FILE *fichier, Elf32_Shdr *s, Elf32_Ehdr h);

char *read_type (Elf32_Shdr s);

char *read_flags (char *flag, Elf32_Word s);

void print_section (FILE *file, Elf32_Shdr *T, Elf32_Ehdr *e);

void get_sh_values(Elf32_Shdr **sectionHeader, FILE* f,Elf32_Ehdr *h);

int main(int argc, char *argv[]);










