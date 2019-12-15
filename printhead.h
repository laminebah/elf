#ifndef _H_PHEAD_H
#define _H_PHEAD_H

/****gère l'affichage de l'entete du fichier*****/
/**structure pour géner les différents strings à afficher***/

typedef struct {
    int i;
    const char *chaine;
}stringTypes;

void print_e_ident(Elf32_Ehdr *entete);
void print_type(Elf32_Ehdr *entete);
void print_classe_type(Elf32_Ehdr *entete);
void print_codage_type(Elf32_Ehdr *entete);
void print_v_machine(Elf32_Ehdr *entete);
void print_OS(Elf32_Ehdr *entete);
void print_e_version(Elf32_Ehdr *entete);
void print_version (Elf32_Ehdr *entete);
void print_header(Elf32_Ehdr *entete);

#endif