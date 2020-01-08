#ifndef __PRINT_CONTENT_SECTION_H__
#define __PRINT_CONTENT_SECTION_H__
#include <elf.h>
#include <stdio.h>
#include <ctype.h>
#define MAX 16
#define SIZENAME 30

							/*  gère l'affichage du contenu d'une section   */



/**Cette fonction récupère le nom d'une section par son indice
 * Données : l'entet elf,la table de section,l'indice idx puis un stream ouvert en lecture
 * -Résultat : un string qui est le nom de la section de l'indice idx
 * **/
char* get_section_name(Elf32_Ehdr* header, Elf32_Shdr* sections_table, int idx, FILE* fichier);

/***	Cette fonction test l' existance d' une section 
		Données : table des sections, l'entete , le nom d'une section et un fichier
		Résultat: entier(num de la section 'name' )
***/
int existe_section(Elf32_Shdr*, Elf32_Ehdr*, char * , FILE* );


/***	Cette fonction affiche le contenu brute d'une section
		Données : une chaine de caractere , table des sections, l'entete ,un fichier ELF et le nom d'une section
		Résultat: aucun
***/
void affiche_contentSection(unsigned char *, Elf32_Shdr*, Elf32_Ehdr*, FILE *, char*);



/***	Cette fonction est utilsée par la fonction lire_contentsec  pour faire l`affichage en format hexa
		Données : une addresse, un tampon, taille et une longeur
		Résultat: aucun
***/
int hexdump(int,unsigned char[],int , int );




/***	Cette fonction est utilsée par la fonction affiche_contentSection, permet de lire une chaine à partir d`une addresse 
		Données :  un fichier ELF, une adresse et une taille
		Résultat: aucun
***/
void lire_contentsec(FILE *,int,int);



/***	Cette fonction permet de lire le contenu d`une section 
		Données : nom d'une section, table des sections, l'entete , un fichier ELF et un boolean
		Résultat: chaine de caractére
***/
unsigned char *print_content_section(char*, Elf32_Shdr*, Elf32_Ehdr*, FILE *, int);

#endif
