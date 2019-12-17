
#ifndef _H_SYMB
#define _H_SYMB
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

/*typedef struct {
	int num;//numéro de la table
	int nbsymb;//nombre de symboles
	char *nom;//nom de la table
	char *symbnom;//noms de toutes les tables
	Elf32_Sym **tab;//table des symboles
}tables_symb;*/


//la fonction renvoie NULL si y a erreur
//elle prend en paramètre le fichier elf ,la table des sections ,l'entete ,une référence sur le nombre de symbole,l'indexe de la section symbtab
//index : indice du N° de la section
//nbsymb : nombre d'entrée dans la table des symboles
Elf32_Sym *lecture_symb(FILE *felf,Elf32_Shdr *tab_section,Elf32_Ehdr elf_head,int *nbsymb,int *index);
void affiche_symbole_table (Elf32_Sym *symbtab,int nbsymb,int index,Elf32_Shdr *tab_section,FILE *felf);
#endif
