#ifndef __PRINT_CONTENT_SECTION_H__
#define __PRINT_CONTENT_SECTION_H__



#include <elf.h>
#include <stdio.h>
#include <ctype.h>


#define N 16
#define BUFF 8196
#define ABS32(S, A, T)	(S + A)| T
#define ABS16(S, A)	(S + A)
#define ABS8(S, A)	(S + A)
#define CALL(S, A, T, P)	((S + A)| T) - P
#define JUMP24(S, A, T, P)	((S + A)| T) - P


int existe_section(unsigned char * sectionHexa, Elf32_Shdr* sectionHeader, Elf32_Ehdr* header, char * name);
void affiche_contentSection(unsigned char * sectionHexa, Elf32_Shdr* section, Elf32_Ehdr* header, FILE *fichier, char* name);
void hexdump(FILE*,int,int);
void print_content_section(char* name, Elf32_Shdr* section ,Elf32_Ehdr* entete, FILE *fichier);


#endif
