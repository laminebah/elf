#include "Elf_header.h"
#include <stdio.h>
#include <stdlib.h>
#include "printhead.h"
#include <unistd.h>
#include <string.h>
#include "section_header.h"
#include "print_content_section.h"
#include "Elf_symbole.h"
#include "print_symbole.h"
#define A  0 //toutes les commandes
#define H  1 //entete
#define X  2 //une section avec numéro ou name
#define XS 3 //table section
#define R  4 //relocation
#define S  5 //symbole

void freemem (void *adr) {
	if (adr != NULL) free (adr);
}

int main (int argc,char **argv) { 
	int option=0,choix=0;
	while ((option=getopt(argc,argv,"ahxsrS:")) != -1) {
		switch (option) {
			case 'a':
					//all
					choix = 0;
				break;
			case 'h':
					//entete
					choix = 1;
				break;
			case 'x':
					//une section
					choix = 2;
				break;
			case 's':
					//symbole
					choix = 5;
				break;
			case 'r':
					//relocation
					choix = 4;
				break;
			case 'S':
					//table section
					choix = 3;
				break;
			default:
				return EXIT_FAILURE;
				
		}
	}
	FILE *felf;
	if (choix == X) {
		if(argc < 4) {
			fprintf(stderr, "Nombre d'argument incorrect \n");
			fprintf(stderr, "Usage: %s fichier \n", argv[0]);
			exit(1);
		}
		//printf ("%s\n",argv[3]);
		felf = fopen (argv[3],"r");
	} else {
		if(argc < 3) {
			fprintf(stderr, "Nombre d'argument incorrect \n");
			fprintf(stderr, "Usage: %s fichier \n", argv[0]);
			exit(1);
		}
		//sprintf ("%s\n",argv[2]);
		felf = fopen (argv[2],"r");
	}
	if (felf == NULL) {
		if (choix == X) printf("readelf::- ERREUR: « %s » n'est pas un fichier ordinaire\n",argv[3]);
		else printf("readelf::- ERREUR:  « %s » n'est pas un fichier ordinaire\n",argv[2]);
		exit(1);		
	}	
	char *namesection;

	//déclaration des tructures
	Elf32_Ehdr * elf_head;
	Elf32_Shdr *sectionHeader;
	Elf32_Sym *symbtab;
	int nbsymb=0,index = 0;	

	elf_head = lecture_entete (felf);
	if (elf_head == NULL)
		exit(1);
	
	sectionHeader=malloc(elf_head->e_shnum*sizeof(Elf32_Shdr));
	get_sh_values(&sectionHeader,felf,elf_head);
	symbtab = lecture_symb(felf,sectionHeader,*elf_head,&nbsymb,&index);
	//exécution commande option
	switch (choix) {
		case A:
			//all : A compléter
			print_header(elf_head);
			print_section(felf,sectionHeader,elf_head);
			affiche_symbole_table (symbtab,nbsymb,index,sectionHeader,felf);
			break;
		case XS:
			//table section : A compléter
			print_section(felf,sectionHeader,elf_head);
			break;
		case H:
			//entete :
			print_header(elf_head);
			break;
		case S:
			//symbole : A compléter
			affiche_symbole_table (symbtab,nbsymb,index,sectionHeader,felf);
			break;
		case X:
			//contenu_section : A compléter
								//une section : soit par N° section ou Name sections
								//si c'est entier on affiche à l'aide du numéro sinon à l'aide du nom
			
			namesection=malloc(sizeof(char)*strlen(argv[2])+1);
			if (namesection == NULL) return EXIT_FAILURE;
			strcpy(namesection,"");
			strcat(namesection,argv[2]);
			print_content_section(namesection, sectionHeader, elf_head, felf);
			break;

		case R:
			//relocation
		/*******A compléter*******/
			break;

	}
	freemem (elf_head);
	freemem (sectionHeader);
	freemem (symbtab);
	fclose (felf);
	return EXIT_SUCCESS;
}