#include "Elf_header.h"
#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "util.h"

// -1 en cas d'erreur si les nombres magiques sont bons
int ElfId (Elf32_Ehdr * elf_head) {
	int resultat  = 0;
	if (elf_head->e_ident[EI_MAG0] != ELFMAG0 || elf_head->e_ident[EI_MAG1] != ELFMAG1 ||
        elf_head->e_ident[EI_MAG2] != ELFMAG2 || elf_head->e_ident[EI_MAG3] != ELFMAG3)
            resultat = -1;
	return resultat;
}
//lecture de l'entête elf pour initialiser la structure NULL en cas d'erreur
Elf32_Ehdr *lecture_entete (FILE *felf) {
	Elf32_Ehdr *elf_head = malloc (sizeof(Elf32_Ehdr));
	if (elf_head == NULL) {
		fprintf (stderr,"readelf : header : erreur\n");
		return elf_head;
	}
    rewind (felf);
    /************e_ident*********/
	fread (elf_head->e_ident,1,EI_NIDENT,felf);
    if (ElfId (elf_head) < 0) {
        fprintf (stderr,"readelf: ERREUR: N'est pas un fichier ELF – a les mauvais octets magiques au départ\n");
        elf_head = NULL;
        return elf_head;
    }
     /***********e_type***********/
    fread (&(elf_head->e_type),sizeof(elf_head->e_type),1,felf);
    /***********e_machine********/
    fread (&(elf_head->e_machine),sizeof(elf_head->e_machine),1,felf);
    /***********e_version********/
    fread (&(elf_head->e_version),sizeof(elf_head->e_version),1,felf);
    /***********e_entry**********/
    fread (&(elf_head->e_entry),sizeof(elf_head->e_entry),1,felf);
    /***********e_phoff**********/
    fread (&(elf_head->e_phoff),sizeof(elf_head->e_phoff),1,felf);
    /***********e_shoff**********/
    fread (&(elf_head->e_shoff),sizeof(elf_head->e_shoff),1,felf);
    /***********e_flags**********/
    fread (&(elf_head->e_flags),sizeof(elf_head->e_flags),1,felf);
    /***********e_ehsize*********/
    fread (&(elf_head->e_ehsize),sizeof(elf_head->e_ehsize),1,felf);
    /***********e_phentsize******/
    fread (&(elf_head->e_phentsize),sizeof(elf_head->e_phentsize),1,felf);
    /***********e_phnum**********/
    fread (&(elf_head->e_phnum),sizeof(elf_head->e_phnum),1,felf);
    /***********e_shentsize******/
    fread (&(elf_head->e_shentsize),sizeof(elf_head->e_shentsize),1,felf);
    /***********e_shnum**********/
    fread (&(elf_head->e_shnum),sizeof(elf_head->e_shnum),1,felf);
    /***********e_shstrndx*******/
    fread (&(elf_head->e_shstrndx),sizeof(elf_head->e_shstrndx),1,felf);    
    if (elf_head->e_ident[EI_DATA] == ELFDATA2MSB) {
		elf_head -> e_type    = __bswap_16 (elf_head -> e_type);
		elf_head -> e_machine = __bswap_16 (elf_head -> e_machine);
		elf_head -> e_version = __bswap_32 (elf_head -> e_version);
		elf_head -> e_entry   = __bswap_32 (elf_head -> e_entry);
		elf_head -> e_phoff   = __bswap_32 (elf_head -> e_phoff);
		elf_head -> e_shoff   = __bswap_32 (elf_head -> e_shoff);
		elf_head -> e_flags   = __bswap_32 (elf_head -> e_flags);
		
		elf_head -> e_ehsize    = __bswap_16 (elf_head -> e_ehsize);
		elf_head -> e_phentsize = __bswap_16 (elf_head -> e_phentsize);
		elf_head -> e_phnum     = __bswap_16 (elf_head -> e_phnum);
		elf_head -> e_shentsize = __bswap_16 (elf_head -> e_shentsize);
		elf_head -> e_shnum     = __bswap_16 (elf_head   -> e_shnum);
		elf_head -> e_shstrndx  = __bswap_16 (elf_head -> e_shstrndx);
		
	}
    return elf_head;
}

