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


/**
	* Récupère le contenu de la table des réalocations et le renvoie
	* Paramètres : 	h une structure de type Elf32_Ehdr
					S une structure de type Elf32_Shdr
					file le fichier qu'on lit
					index l'index de position
	* Retourne une table de réalocations
**/
Elf32_Rel *read_REL_tab(Elf32_Ehdr h, Elf32_Shdr *S, FILE *file, int index);



/**
	* Affiche la 1ère ligne de la table de réalocations
	* Paramètres : 	RelSize la taille de la table
					sh_name le nom de la section
					sh_offset le déccalage de la section
**/
void print_RelTab_head(int RelSize, unsigned char *sh_name, unsigned int sh_offset);



/**
	* Retourne la chaîne de caractères qui correspond au type de la table de réalocations
	* Paramètres :	REL_tab la table de réalocations qu'on lit
**/
char * reloc_types(Elf32_Rel REL_tab);



/**
	* Retourne le type du symbole sous forme de chaîne de caractères
	* Paramètres :	type le type du symbole qu'on lit
**/
char* get_sym_type (unsigned int type);



/**
	* Affiche le contenu de la table de réalocations
	* Paramètres : 	Sec une structure de type Elf32_Shdr
					h une structure de type Elf32_Ehdr
					Sym une structure de type Elf32_Sym
					file le fichier qu'on lit
**/
void affiche_Relocation(Elf32_Shdr*Sec, Elf32_Ehdr h, Elf32_Sym *Sym, unsigned char *strtab, FILE *file);



/**
	* Renvoie le nom d'une table des symboles sous forme de chaîne de caractères
	* Paramètres : 	Sec une structure de type Elf32_Shdr
					index l'index de position
					file le fichier qu'on lit
**/
char* SymName(Elf32_Shdr *Sec, int index, int indexStrTab, FILE *file);

#endif

