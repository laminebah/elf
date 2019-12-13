#include "Elf_header.h"
#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "util.h"

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
    return elf_head;
}

//ajout ici

//afficheur Magique
void print_e_ident(Elf32_Ehdr *entete)
{
    for(int i=0; i<16; i++)
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
        case 62: printf ("Advanced Micro Devices X86-64");
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
        printf(" UNIX - System V");
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
        printf("0x0\n");
	}
    if(entete->e_version == EV_CURRENT)
	{
        printf("0x1\n");
	}
}

//affichage de la version courant
void print_version (Elf32_Ehdr *entete) {
	if (entete -> e_ident [EI_VERSION] == EV_NONE) 
        printf ("%d (invalid)\n",entete -> e_ident [EI_VERSION]);
	if (entete -> e_ident [EI_VERSION] == EV_CURRENT) 
        printf ("%d (current)\n",entete -> e_ident [EI_VERSION]);
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
    printf("Adresse du point d'entrée:\t\t      0x%0x \n", entete->e_entry);
    printf("Début des en-tetes de programme : \t %d (octets dans le fichier)\n", entete->e_phoff);
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
