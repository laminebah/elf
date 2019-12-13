#include "Elf_header.h"
#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// -1 en cas d'erreur si les nombres magiques sont bons
int ElfId (FILE *felf,Elf32_Ehdr * elf_head) {
	int resultat  = 0;
	int c;
	//
	if ((c=fgetc(felf)) < 0) return -1;
	if (c != ELFMAG0) return -1;
	elf_head -> e_ident [EI_MAG0] = c;
	//
	if ((c=fgetc(felf)) < 0) return -1;
	if (c != ELFMAG1) return -1;
	elf_head -> e_ident [EI_MAG1] = c;
	//
	if ((c=fgetc(felf)) < 0) return -1;
	if (c != ELFMAG2) return -1;
	elf_head -> e_ident [EI_MAG2] = c;
	//
	if ((c=fgetc(felf)) < 0) return -1;
	if (c != ELFMAG3) return -1;
	elf_head -> e_ident [EI_MAG3] = c;
	return resultat;
}
//lecture de l'entête elf pour initialiser la structure NULL en cas d'erreur
Elf32_Ehdr *lecture_entete (FILE *felf) {
	Elf32_Ehdr *elf_head = malloc (sizeof(Elf32_Ehdr));
	int c;
	if (elf_head == NULL) {
		fprintf (stderr,"Erreur(1) Allocation de l'entete\n");
		return elf_head;
	}
/***********************************e_ident*********************************************/
	//EI_MAG0...3
	if (ElfId (felf,elf_head) < 0) {
		fprintf (stderr,"readelf: ERREUR: N'est pas un fichier ELF – a les mauvais octets magiques au départ\n");
		elf_head = NULL;
		return elf_head;
	}
	//EI_CLASS
	if ((c= fgetc (felf)) == EOF) {
		fprintf (stderr,"readelf: Erreur: Fin du fichier atteinte\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head -> e_ident [EI_CLASS] = c;
	//EI_DATA
	if ((c= fgetc (felf)) == EOF) {
		fprintf (stderr,"readelf: Erreur: Fin du fichier atteinte\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head -> e_ident [EI_DATA] = c;
	//EI_VERSION
	if ((c= fgetc (felf)) == EOF) {
		fprintf (stderr,"readelf: Erreur: Fin du fichier atteinte\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head -> e_ident [EI_VERSION] = c;
	//EI_PAD
	if ((c= fgetc (felf)) == EOF) {
		fprintf (stderr,"readelf: Erreur: Fin du fichier atteinte\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head -> e_ident [EI_PAD] = c;
	//EI_NIDENT
	if ((c= fgetc (felf)) == EOF) {
		fprintf (stderr,"readelf: Erreur: Fin du fichier atteinte\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head -> e_ident [EI_NIDENT] = c;
/************************************e_type*********************************/
	if (fread (&(elf_head->e_type),2,1,felf) != 1) {
		fprintf (stderr,"readelf: Erreur: e_type\n");
		elf_head = NULL;
		return elf_head;
	}
/************************************e_machine******************************/
	if (fread (&(elf_head->e_machine),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_machine\n");
		elf_head = NULL;
		return elf_head;
	}
/************************************e_version******************************/
	if (fread (&(elf_head->e_version),4,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_version\n");
		elf_head = NULL;
		return elf_head;
	}
/************************************e_entry******************************/
	if (fread (&(elf_head->e_entry),4,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_entry\n");
		elf_head = NULL;
		return elf_head;
	}
/************************************e_phoff******************************/
	if (fread (&(elf_head->e_phoff),4,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_phoff\n");
		elf_head = NULL;
		return elf_head;
	}
/************************************e_shoff******************************/
	if (fread (&(elf_head->e_shoff),4,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_shoff\n");
		elf_head = NULL;
		return elf_head;
	}
/************************************e_flags******************************/
	if (fread (&(elf_head->e_flags),4,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_flags\n");
		elf_head = NULL;
		return elf_head;
	}
/************************************e_ehsize******************************/
	if (fread (&(elf_head->e_ehsize),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_ehsize\n");
		elf_head = NULL;
		return elf_head;
	}

/************************************e_phentsize******************************/
	if (fread (&(elf_head->e_phentsize),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_phentsize\n");
		elf_head = NULL;
		return elf_head;
	}

/************************************e_phnum******************************/
	if (fread (&(elf_head->e_phnum),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_phnum\n");
		elf_head = NULL;
		return elf_head;
	}
/************************************e_shentsize******************************/
	if (fread (&(elf_head->e_shentsize),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_shentsize\n");
		elf_head = NULL;
		return elf_head;
	}
/************************************e_shnum******************************/
	if (fread (&(elf_head->e_shnum),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_shnum\n");
		elf_head = NULL;
		return elf_head;
	}
/************************************e_shstrndx******************************/
	if (fread (&(elf_head->e_shstrndx),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_shstrndx\n");
		elf_head = NULL;
		return elf_head;
	}

	return elf_head;
}
