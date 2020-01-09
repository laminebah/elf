#ifndef _H_RELOC
#define _H_RELOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <byteswap.h>
#include <assert.h>
#include "Elf_header.h"
#include "section_header.h"


//recupere le contenu de la table de realocation et le renvoie
Elf32_Rel *read_REL_tab(Elf32_Ehdr h, Elf32_Shdr *S, FILE *file, int index);

//un simple affiche de la premiere ligne du tableau de realocation
void print_RelTab_head(int RelSize, unsigned char *sh_name, unsigned int sh_offset);

//retourne la chaine de caracteres qui correspond au type de la tble de realocation
char * reloc_types(Elf32_Rel REL_tab);

//retourne le type de la table des symboles qui correspond comme chaine de caracteres
char* get_sym_type (unsigned int type);

//un affichage du contenu de la table de realocation
void affiche_Relocation(Elf32_Shdr*Sec, Elf32_Ehdr h, Elf32_Sym *Sym, unsigned char *strtab, FILE *file);

//renvoie le nom d'une table des symboles comme chaine de caracteres
char* SymName(Elf32_Shdr *Sec, int index, int indexStrTab, FILE *file);

#endif

