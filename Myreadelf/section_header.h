#ifndef _H_SECH_
#define _H_SECH_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <byteswap.h>
#include <assert.h>
#include "Elf_header.h"

#define SIZENAME 100


/**
  * Retourne l'entier passé en paramètre en little endian
  * Paramètres :  h une entête
                  value un entier sur 32 bits
**/
int32_t val_32(Elf32_Ehdr h, int32_t value);



/**
  * Retourne l'entier passé en paramètre en little endian
  * Paramètres :  h une entête 
                  value un entier sur 16 bits
**/
int16_t val_16(Elf32_Ehdr h, int16_t value);



/**
  * Affecte les bonne valeurs (en little endian) de la section du fichier f à la section s
  * Paramètres :  fichier le fichier qu'on lit 
                  S une section
                  h une entête
**/
void read_section(FILE *fichier, Elf32_Shdr *s, Elf32_Ehdr h);



/**
  * Retourne le type de la section s sous forme de chaîne de caractères
  * Paramètres : s une section
**/
char *read_type (Elf32_Shdr s);



/**
  * Retourne le flag de la section s sous forme de chaîne de caractères
  * Paramètres :  flag le  flag qu'on récupère pour lire
                  s une section 
**/
char *read_flags (char *flag, Elf32_Word s);



/**
  * Affiche la section T d'un fichier file
  * Paramètres :  file le fichier qu'on lit
                  T une section
                  e une structure de type Elf32_Ehdr
**/
void print_section (FILE *file, Elf32_Shdr *T, Elf32_Ehdr *e);



/**
  * Récupère toutes les valeurs de la section en little endian et les mets dans sectionHeader
  * Paramètres :  sectionHeader la section dans laquelle on met les valeurs récupérées
                  f le fichier qu'on lit
                  h une structure de type Elf32_Ehdr
**/
void get_sh_values(Elf32_Shdr **sectionHeader, FILE* f,Elf32_Ehdr *h);

#endif

