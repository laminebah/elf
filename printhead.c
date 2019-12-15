#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "printhead.h"


void print_e_ident(Elf32_Ehdr *entete)
{
    for(int i=0; i<16; i++)
        printf("%02x ", entete->e_ident[i]);
    printf("\n");
}

void print_type(Elf32_Ehdr *entete)
{
    stringTypes type [] = { {ET_NONE, "Aucun"},                    {ET_REL , "REL (fichier de réadressage)"},
                            {ET_EXEC, "EXEC (fichier exécutable)"},{ET_DYN,  "DYN (fichier objet partagé)"},
                            {ET_CORE, "CORE (fichier Core)"},      {ET_LOPROC  ,"LOPROC"},
                            {ET_HIPROC,"HIPROC"},                  {0xFFFFFFFF,"Inconnue"}
    };
    printf ("%s\n",type[entete->e_type].chaine);
}

//affichage du type de classe
void print_classe_type(Elf32_Ehdr *entete)
{   
   stringTypes class [] = { {ELFCLASSNONE,"Aucune"},{ELFCLASS32,  "ELF32"},
                            {ELFCLASS64,  "ELF64"}, {0xFFFFFFFF,"Inconnue"}
   };
   printf ("%s\n",class[entete->e_ident[EI_CLASS]].chaine);
}

//affichage du type de codage
void print_codage_type(Elf32_Ehdr *entete)
{
    stringTypes data [] = { {ELFDATANONE,"Aucune"},
                            {ELFDATA2LSB,"complément à 2, système à octets de poids faible d'abord (little endian)"},
                            {ELFDATA2MSB,"complément à 2, système à octets de poids faible après (big endian)"},
                            {0xFFFFFFFF,"Inconnue"}
    };
	printf("%s\n",data[entete->e_ident[EI_DATA]].chaine);
}

//affichhage architecture machine
void print_v_machine(Elf32_Ehdr *entete)
{
    switch(entete->e_machine)
    {
        case ET_NONE:   printf("No machine");
            break;
        case EM_M32:    printf("AT&T WE 32100");
            break;
        case EM_SPARC:  printf("SUN SPARC");
            break;
        case EM_386:    printf("Intel 80386");
            break;
        case EM_68K:    printf("Motorola m68k family");
            break;
        case EM_88K:    printf("Motorola m88k family");
            break;
        case EM_860:    printf("Intel 80860");
            break;
        case EM_MIPS:   printf("MIPS R3000 big-endian");
            break;
        case EM_MIPS_RS3_LE:   printf("MIPS RS4000 little-endian");
            break;
        case 62:               printf ("Advanced Micro Devices X86-64");
            break;
        case EM_ARM:           printf ("ARM");
            break;
        default:               printf("RESERVED");
            break;
    }
    printf("\n");
}

//print OS
void print_OS(Elf32_Ehdr *entete)
{
    if(entete->e_ident[EI_OSABI] == ELFOSABI_SYSV)
        printf("UNIX - System V");
    if(entete->e_ident[EI_OSABI] == ELFOSABI_ARM)
        printf("ARM");
    if(entete->e_ident[EI_OSABI] == ELFOSABI_LINUX)
        printf("LINUX - System");
    if(entete->e_ident[EI_OSABI] == ELFOSABI_TRU64)
        printf("UNIX TRU64");
    printf("\n");
}

//affichage version
void print_e_version(Elf32_Ehdr *entete)
{
     stringTypes eversion [] = {{EV_NONE,"0x0"},{EV_CURRENT,"0x1"}

    };
    printf ("%s\n",eversion[entete->e_version].chaine);
}

//affichage de la version courant
void print_version (Elf32_Ehdr *entete) {
	 stringTypes version [] = {{EV_NONE,"(invadlid)"},{EV_CURRENT,"(current)"}

     };
    printf ("%d %s\n",entete->e_ident[EI_VERSION],version[entete->e_ident[EI_VERSION]].chaine);
}

void print_space (int n) {
    for (int i = 0; i < n; i ++)
        printf (" ");
}
//affichage de l'entete
void print_header(Elf32_Ehdr *entete)
{
    printf("En-tête ELF:\n");
    printf("  Magique:"); print_space(3);  print_e_ident(entete);
    printf("  Classe:");  print_space(28); print_classe_type(entete);
    printf("  Données:"); print_space(26); print_codage_type(entete);
    printf("  Version:"); print_space(27); print_version(entete);
    printf("  OS/ABI:");  print_space(28); print_OS(entete);
    printf("  Version ABI:"); print_space(23); printf("%d\n", entete->e_ident[EI_ABIVERSION]);
    printf("  Type:");    print_space(30); print_type(entete);
    printf("  Machine:"); print_space(27); print_v_machine(entete);
    printf("  Version:"); print_space(27); print_e_version(entete);
    printf("  Adresse du point d'entrée:"); print_space(15); printf("0x%0x\n", entete->e_entry);
    printf("  Début des en-têtes de programme :");print_space(10);printf("%d (octets dans le fichier)\n", entete->e_phoff);
    printf("  Début des en-têtes de section :");print_space(10);printf("%d (octets dans le fichier)\n", entete->e_shoff);
    printf("  Fanions:");print_space(27);printf("0x%0x\n", entete->e_flags);
    printf("  Taille de cet en-tête:");print_space(13);printf("%d (octets)\n", entete->e_ehsize);
    printf("  Taille de l'en-tête du programme:");print_space(2);printf("%d (octets)\n", entete->e_phentsize);
    printf("  Nombre d'en-tête du programme:");print_space(5);printf("%d\n", entete->e_phnum);
    printf("  Taille des en-têtes de section:");print_space(4);printf("%d (octets)\n", entete->e_shentsize);
    printf("  Nombre d'en-têtes de section:");print_space(6);printf("%d\n", entete->e_shnum);
    printf("  Table d'indexes des chaînes d'en-tête de section:");print_space(1);printf("%d\n", entete->e_shstrndx);
}
