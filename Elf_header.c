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

//ajout ici
#include "Elf_header.h"

//afficheur Magique
void print_e_ident(Elf32_Ehdr *entete)
{
    for(int i=0; i<EI_NIDENT; i++)
        printf("%02x ", entete->e_ident[i]);
    printf("\n");
}

//affichage du type
void print_type(Elf32_Ehdr *entete)
{
    switch (entete->e_type)
    {
        case ET_NONE:   printf("NONE");
            break;
        case ET_REL:    printf("REL");
            break;
        case ET_EXEC:   printf("EXEC");
            break;
        case ET_DYN:    printf("DYN");
            break;
        case ET_CORE:   printf("CORE");
            break;
        case ET_LOPROC: printf("LOPROC");
            break;
        case ET_HIPROC: printf("HIPROC");
            break;
        default:        printf(" ");
            break;
    }
    printf("\n");
}

//affichage du type de classe
void print_classe_type(Elf32_Ehdr *entete)
{
    switch (entete->e_ident[EI_CLASS])
    {
        case ELFCLASSNONE:  printf("Invalid Type Encoding\n");
            break;
        case ELFCLASS32: printf("ELF32");
            break;
        case ELFCLASS64: printf("ELF64");
        default: 
            break;
        }
}

//affichage du type de codage
void print_codage_type(Elf32_Ehdr *entete)
{
    switch(entete->e_ident[EI_DATA])
    {
        case ELFDATANONE: printf("Invalid data encoding");
            break;
        case ELFDATA2LSB: printf("Complement à 2, systeme à octets de poids fort d'abord (big endian)");
            break;
        case ELFDATA2MSB: printf("Complement à 2, systeme à octets de poids faible d'abord (little endian)");
            break;
        default:
            break;
    }
}

//affichhage architecture machine
void print_v_machine(Elf32_Ehdr *entete)
{
    switch(entete->e_machine)
    {
        case ET_NONE:   printf("No Machine");
            break;
        case EM_M32:   printf("AT&T WE 32100");
            break;
        case EM_SPARC:   printf("SPARC");
            break;
        case EM_386:   printf("Intel Architecture");
            break;
        case EM_68K:   printf("Motorola 68000");
            break;
        case EM_88K:   printf("Motorola 88000");
            break;
        case EM_860:   printf("Intel 80860");
            break;
        case EM_MIPS:   printf("MIPS RS3000 Big-Endian");
            break;
        case EM_MIPS_RS3_LE:   printf("MIPS RS4000 Big-Endidan");
            break;
        default: printf("RESERVED");
            break;
    }
}

//print OS

//affichage version
void print_e_version(Elf32_Ehdr *entete)
{
    if(entete->e_version == EV_NONE)
        printf("NONE");
    else if(entete->e_version == EV_CURRENT)
        printf("CURRENT");
    else 
        printf(" ");
}

//affichage de l'entete
void print_header(Elf32_Ehdr *entete)
{
    printf("En-tête ELF: \n");
    printf("Magique: ");        print_e_ident(entete);
    printf("Classe: ");         print_classe_type(entete);
    printf("Données: ");        print_codage_type(entete);
    printf("Version: ");        print_e_version(entete);
    printf("OS/ABI: \n");
    printf("Version ABI: \n");
    printf("Type: \n");         print_type(entete);
    printf("Machine: \n");      print_v_machine(entete);
    printf("Version: \n");
    printf("Adresse du point d'entrée: \n");
    printf("Début des en-tetes de programmes: \n");
    printf("Début des en-tetes de section: \n");
    printf("Fanions: \n");
    printf("Taille de cet en-tete: \n");
    printf("Taille de l'entete du programme: \n");
    printf("Nombre d'en-tete du programme: \n");
    printf("Taille des en-tetes de section: \n");
    printf("Nombre d'en-tete de section: \n");
    printf("Table d'index des chaines d'en-tete de section: \n");
}




