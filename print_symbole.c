#include "Elf_symbole.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "section_header.h"
#include <byteswap.h>

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
	for (int i = 0; i < *nbsymb ; i ++) {
		
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

