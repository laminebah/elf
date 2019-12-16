#ifndef __PRINT_CONTENT_SECTION_H__
#define __PRINT_CONTENT_SECTION_H__



#include <elf.h>
#include <stdio.h>

#define N 16
#define BUFF 8196
#define ABS32(S, A, T)	(S + A)| T
#define ABS16(S, A)	(S + A)
#define ABS8(S, A)	(S + A)
#define CALL(S, A, T, P)	((S + A)| T) - P
#define JUMP24(S, A, T, P)	((S + A)| T) - P



// typedef struct{
// 	Elf32_Ehdr header;
// 	Elf32_Shdr *section;
// 	unsigned char *sectionHexa;
// 	Elf32_Sym  *symtab;
// 	unsigned char *symtable;
// 	Elf32_Rel  *reltab;	
// }Elf32_info;


int get_section_num(Elf32_Shdr* sectionHeader, Elf32_Ehdr* header, char * name);

//void affiche_contentSection(Elf32_Shdr sectionHexa, Elf32_Shdr* section, Elf32_Ehdr* header, FILE *file, char* name);

void myhexdump(FILE*,int,int);

//void content_section(char* name, Elf32_Shdr* section ,Elf32_Ehdr* entete, FILE *fichier);
// void setup_little_endian(Elf32_info *elf);

// void initElf(Elf32_info *elf,FILE *file);

// void lire_Section_table(Elf32_info *,FILE *);





#endif