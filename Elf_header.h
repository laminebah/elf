#ifndef _H_ELF_HEADER_
#define _H_ELF_HEADER_
#include <elf.h>
#include <stdio.h>
int ElfId (FILE *felf,Elf32_Ehdr * elf_head);
Elf32_Ehdr *lecture_entete (FILE *felf);

//ajout a partir dici 
//affichage du type
void print_type(Elf32_Ehdr *entete);

//affichage du type de classe 
void print_classe_type(Elf32_Ehdr *entete);

//affichage du type de codage
void print_codage_type(Elf32_Ehdr *entete);

//affichhage architecture machine
void print_v_machine(Elf32_Ehdr *entete);

//affichage version
void print_e_version(Elf32_Ehdr *entete);

//affichage de l'entete
void print_header(Elf32_Ehdr *entete);

//print OS
void print_OS(Elf32_Ehdr *entete);

#endif 
