#include "Elf_header.h"
#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int32_t value_32(int32_t value)
{
	if(is_big_endian())
		return __bswap_32(value);
	return value;
}

int16_t value_16(int16_t value)
{
	if(is_big_endian())
		return __bswap_16(value);
	return value;
}

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
	elf_head ->e_type = value_16 (elf_head->e_type);
/************************************e_machine******************************/
	if (fread (&(elf_head->e_machine),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_machine\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head ->e_machine = value_16 (elf_head->e_machine);
/************************************e_version******************************/
	if (fread (&(elf_head->e_version),4,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_version\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head ->e_version = value_32 (elf_head->e_version);
/************************************e_entry******************************/
	if (fread (&(elf_head->e_entry),4,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_entry\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head ->e_entry = value_32 (elf_head->e_entry);
/************************************e_phoff******************************/
	if (fread (&(elf_head->e_phoff),4,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_phoff\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head ->e_phoff = value_32 (elf_head->e_phoff);
/************************************e_shoff******************************/
	if (fread (&(elf_head->e_shoff),4,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_shoff\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head ->e_shoff = value_32 (elf_head->e_shoff);
/************************************e_flags******************************/
	if (fread (&(elf_head->e_flags),4,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_flags\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head ->e_flags = value_32 (elf_head->e_flags);
/************************************e_ehsize******************************/
	if (fread (&(elf_head->e_ehsize),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_ehsize\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head ->e_ehsize = value_16 (elf_head->e_ehsize);
/************************************e_phentsize******************************/
	if (fread (&(elf_head->e_phentsize),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_phentsize\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head ->e_phentsize = value_16 (elf_head->e_phentsize);
/************************************e_phnum******************************/
	if (fread (&(elf_head->e_phnum),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_phnum\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head ->e_phnum = value_16 (elf_head->e_phnum);
/************************************e_shentsize******************************/
	if (fread (&(elf_head->e_shentsize),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_shentsize\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head ->e_shentsize = value_16 (elf_head->e_shentsize);
/************************************e_shnum******************************/
	if (fread (&(elf_head->e_shnum),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_shnum\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head ->e_shnum = value_16 (elf_head->e_shnum);
/************************************e_shstrndx******************************/
	if (fread (&(elf_head->e_shstrndx),2,1,felf) != 1) {
		fprintf (stderr,"readelf : Erreur: e_shstrndx\n");
		elf_head = NULL;
		return elf_head;
	}
	elf_head ->e_shstrndx = value_16 (elf_head->e_shstrndx);

	return elf_head;
}

//ajout ici

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
        case ET_NONE:   printf("NONE (Type du fichier inconnu)");
            break;
        case ET_REL:    printf("REL (Fichier de réadressage) ");
            break;
        case ET_EXEC:   printf("EXEC (EXE (Fichier executable) ");
            break;
        case ET_DYN:    printf("DYN (Objet partagé)");
            break;
        case ET_CORE:   printf("CORE (Objet Core) ");
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
	printf("\n");
}

//affichage du type de codage
void print_codage_type(Elf32_Ehdr *entete)
{
    switch(entete->e_ident[EI_DATA])
    {
        case ELFDATANONE: printf("Invalid data encoding");
            break;
        case ELFDATA2LSB: printf("Complement à 2, systeme à octets de poids faible d'abord (little endian)");
            break;
        case ELFDATA2MSB: printf("Complement à 2, systeme à octets de poids fort d'abord (big endian)");
            break;
        default:
            break;
    }
	printf("\n");
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
	printf("\n");
}

//print OS
void print_OS(Elf32_Ehdr *entete)
{
    if(entete->e_ident[EI_OSABI] == ELFOSABI_SYSV)
        printf("Unix System V ABI");
    if(entete->e_ident[EI_OSABI] == ELFOSABI_ARM)
        printf("Architecture ARM");
    if(entete->e_ident[EI_OSABI] == ELFOSABI_LINUX)
        printf("LINUX - System");
    if(entete->e_ident[EI_OSABI] == ELFOSABI_TRU64)
        printf("UNIX TRU64");
    printf("\n");
    
}

//affichage version
void print_e_version(Elf32_Ehdr *entete)
{
    if(entete->e_version == EV_NONE)
	{
        printf("0\n");
	}
    if(entete->e_version == EV_CURRENT)
	{
        printf("1\n");
	}
	printf(" \n");
}

//affichage de la version courant
void print_version (Elf32_Ehdr *entete) {
	if (entete -> e_ident [EI_VERSION] == EV_NONE) 
        printf ("%d (invalid)\n",entete -> e_ident [EI_VERSION]);
	if (entete -> e_ident [EI_VERSION] == EV_CURRENT) 
        printf ("%d (current)\n",entete -> e_ident [EI_VERSION]);
	printf("\n");
}


//affichage de l'entete
void print_header(Elf32_Ehdr *entete)
{
    printf("En-tête ELF: \n");
    printf("Magique: \t\t");        print_e_ident(entete);
    printf("Classe: \t\t\t\t");         print_classe_type(entete);
    printf("Données: \t\t\t\t");        print_codage_type(entete);
    printf("Version: \t\t\t\t");        print_version(entete);
    printf("OS/ABI: \t\t\t\t");       print_OS(entete);
    printf("Version ABI: \t\t\t\t %d \n", entete->e_ident[EI_ABIVERSION]);
    printf("Type: \t\t\t\t\t");         print_type(entete);
    printf("Machine: \t\t\t\t");      print_v_machine(entete);
    printf("Version: \t\t\t\t");      print_e_version(entete);
    printf("Adresse du point d'entrée: \t\t %0x \n", entete->e_entry);
    printf("Début des en-tetes de programmes: \t %d (octets dans le fichier)\n", entete->e_phoff);
    printf("Début des en-tetes de section: \t\t %d (octets dans le fichier)\n", entete->e_shoff);
    printf("Fanions: \t\t\t\t %0x \n", entete->e_flags);
    printf("Taille de cet en-tete: \t\t\t %d (octets)\n", entete->e_ehsize);
    printf("Taille de l'entete du programme: \t %d (octets) \n", entete->e_phentsize);
    printf("Nombre d'en-tete du programme: \t\t %d \n", entete->e_phnum);
    printf("Taille des en-tetes de section: \t %d (octets) \n", entete->e_shentsize);
    printf("Nombre d'en-tete de section: \t\t %d\n", entete->e_shnum);
    printf("Table d'index des chaines d'en-tete de section: \t %d\n", entete->e_shstrndx);
    printf("\n");
}




