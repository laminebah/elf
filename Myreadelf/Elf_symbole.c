#include "Elf_symbole.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "section_header.h"
#include <byteswap.h>
#include "print_symbole.h"

//lecture et affichage de la table des symboles
Elf32_Sym *lecture_symb(FILE *felf,Elf32_Shdr *tab_section,Elf32_Ehdr elf_head,int *nbsymb,int *index) {
	*index = 0;
	Elf32_Sym *symbtab;
	
	while (*index < elf_head.e_shnum && tab_section [*index].sh_type != SHT_SYMTAB) {
		*index +=1;
	}
	 *nbsymb = tab_section [*index].sh_size / 16;
	assert (*nbsymb != -1);
	symbtab = malloc (sizeof(Elf32_Sym)*(*nbsymb));
	assert(symbtab != NULL);
	int i = 0;
	for (; i < *nbsymb ; i ++) {
		
		fseek (felf,tab_section[*index].sh_offset + i * tab_section [*index].sh_entsize,SEEK_SET);
		assert(fread (&symbtab[i].st_name,sizeof(symbtab[i].st_name),1,felf) == 1);
		symbtab[i].st_name  = val_32(elf_head,symbtab[i].st_name);
		assert(fread (&symbtab[i].st_value,sizeof(symbtab[i].st_value),1,felf) == 1);
		symbtab[i].st_value = val_32(elf_head,symbtab[i].st_value);
		//printf ("%08x\n",symbtab[i].st_value);
		assert(fread (&symbtab[i].st_size,sizeof(symbtab[i].st_size),1,felf) == 1);
		symbtab[i].st_size = val_32(elf_head,symbtab[i].st_size);
		assert(fread (&symbtab[i].st_info,sizeof(symbtab[i].st_info),1,felf) == 1);
		assert(fread (&symbtab[i].st_other,sizeof(symbtab[i].st_other),1,felf) == 1);
		assert(fread (&symbtab[i].st_shndx,sizeof(symbtab[i].st_shndx),1,felf) == 1);
		symbtab[i].st_shndx = val_16(elf_head,symbtab[i].st_shndx);
		//printf ("%d:%3d\n",i,symbtab[i].st_shndx);
	}
	
	return symbtab;
}

void affiche_symbole_table (Elf32_Sym *symbtab,int nbsymb,int index,Elf32_Shdr *tab_section,FILE *felf) {
	
	//
	char *tampon ;
	int linksecsym = tab_section [index].sh_link;
	assert ((tampon=malloc(tab_section[linksecsym].sh_size)) != NULL);
	assert (fseek(felf,tab_section[linksecsym].sh_offset,SEEK_SET) != -1);
	assert(fread (tampon,tab_section[linksecsym].sh_size,1,felf) == 1);
	//assert (fread (tampon,tab_section[linksecsym].sh_size,1,felf) == tab_section[linksecsym].sh_size);
	
	printf("\n");
	printf ("La table de symboles « .symtab » contient %d entrées :\n",nbsymb);
	printf ("   Num:    Valeur Tail Type    Lien   Vis      Ndx Nom\n");
	int i = 0;
	for (; i < nbsymb; i ++) {
		printf("   %3d: ", i);
		printf("%08x  ", symbtab[i].st_value);
		printf("%4d ", symbtab[i].st_size);
		print_type_symb(symbtab[i].st_info);
		print_link(symbtab[i].st_info);
		print_visi(symbtab[i].st_other);
		switch (symbtab[i].st_shndx) {
			case SHN_UNDEF: printf ("UND ");
				break;
			case SHN_ABS:   printf ("ABS ");
				break;
			default :
				printf("%3d ", symbtab[i].st_shndx);
		}
		printf("%s", tampon+symbtab[i].st_name);
		printf("\n");
	}
}
